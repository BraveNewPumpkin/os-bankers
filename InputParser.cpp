//
// Created by Kyle Bolton on 10/7/16.
//

#include "InputParser.h"

void InputParser::parseInput(std::unique_ptr<Bank>& bank, std::unique_ptr<std::vector<std::unique_ptr<Process> > >& processes) {
  auto num_resources = parseNumResources();
  auto num_processes = parseNumProcesses();
  std::unique_ptr<std::vector<unsigned int> > resource_instances = parseInstancesOfResources(num_resources);
  std::unique_ptr<std::vector<std::vector<unsigned int> > > demands = parseDemands(num_processes, num_resources);
  //populate bank
  bank = std::make_unique<Bank>(num_processes, num_resources, demands);
  for(const unsigned int& num_instances: *resource_instances){
    bank->addResource(num_instances);
  }
  processes = parseProcesses(num_processes);
}

unsigned int InputParser::parseNumResources() {
  std::string line;
  std::getline(input, line);
  return matchBareNum(line);
}

unsigned int InputParser::parseNumProcesses() {
  std::string line;
  std::getline(input, line);
  return matchBareNum(line);
}

std::unique_ptr<std::vector<unsigned int> > InputParser::parseInstancesOfResources(const unsigned int& num_resources) {
  auto resource_instances = std::make_unique<std::vector<unsigned int> >();
  for (int i = 0; i < num_resources; ++i) {
    std::string line;
    std::getline(input, line);
    resource_instances->push_back(matchBareNum(line));
  }
  return resource_instances;
}

std::unique_ptr<std::vector<std::vector<unsigned int> > > InputParser::parseDemands(const unsigned int& num_processes, const unsigned int& num_resources) {
  auto demands = std::make_unique<std::vector<std::vector<unsigned int> > >(num_processes, std::vector<unsigned int>(num_resources, 0));
  std::regex demand_parser("max\\[(\\d+),(\\d+)\\]=(\\d+)");
  std::string line;
  std::getline(input, line);
  while(std::regex_search(line, match, demand_parser)){
    auto process_index = std::stoul(match.str(1))-1;
    auto resource_index = std::stoul(match.str(2))-1;
    auto demand = std::stoul(match.str(3));
    demands->at(process_index).at(resource_index) = (unsigned int)demand;
    std::getline(input, line);
  }
  return demands;
}

std::unique_ptr<std::vector<std::unique_ptr<Process> > > InputParser::parseProcesses(const unsigned int& num_processes) {
  auto processes = std::make_unique <std::vector<std::unique_ptr<Process> > >();
  for (unsigned int i = 0; i < num_processes; ++i) {
    processes->push_back(parseProcess(i));
  }
  return processes;
}

std::unique_ptr<Process> InputParser::parseProcess(const unsigned int& index) {
  std::unique_ptr<Process> process = std::make_unique <Process>(index);
  std::regex process_id_parser("process\\D*(\\d+)");
  std::regex calculate_parser("calculate\\((\\d+)\\)");
  std::regex useresources_parser("useresources\\((\\d+)\\)");
  std::regex request_parser("request\\(((?:\\d+,?)+)\\)");
  std::regex release_parser("release\\(((?:\\d+,?)+)\\)");
  std::regex end_parser("end");
  std::string line;
  bool at_end = false;
  //get id from process label
  std::getline(input, line);
  if(std::regex_search(line, match, process_id_parser)) {
    const unsigned int id = (unsigned int)std::stoul(match.str(1));
    process->setId(id);
  }else{
    throw std::runtime_error("failed to parse process id from line: " + line);
  }
  //get deadline and computation time
  std::getline(input, line);
  process->setDeadline(matchBareNum(line));
  std::getline(input, line);
  process->setProcessingTime(matchBareNum(line));
  //parse all instructions
  while(std::getline(input, line) && !at_end){
    if(std::regex_search(line, match, calculate_parser)){
      const unsigned int duration = std::stoul(match.str(1));
      process->pushInstruction(Process::Instruction::calculate, duration);
    }else if(std::regex_search(line, match, useresources_parser)){
      const unsigned int duration = std::stoul(match.str(1));
      process->pushInstruction(Process::Instruction::useresources, duration);
    }else if(std::regex_search(line, match, request_parser)){
      std::vector<unsigned int> requested_resources;
      std::vector<std::string> tokens = split(match.str(1),',');
      for(const std::string& token: tokens){
        const unsigned int num_instances = std::stoul(token);
        requested_resources.push_back(num_instances);
      }
      process->pushInstruction(Process::Instruction::request, requested_resources);
    }else if(std::regex_search(line, match, release_parser)){
      std::vector<unsigned int> requested_resources;
      std::vector<std::string> tokens = split(match.str(1),',');
      for(const std::string& token: tokens){
        const unsigned int num_instances = std::stoul(token);
        requested_resources.push_back(num_instances);
      }
      process->pushInstruction(Process::Instruction::release, requested_resources);
    }else if(std::regex_search(line, match, end_parser)){
      at_end = true;
    }
  }
  return process;
}

unsigned int InputParser::matchBareNum(const std::string& line) {
  std::regex bare_num_parser("^(\\d+)");
  if(std::regex_search(line, match, bare_num_parser)){
    return (unsigned int)std::stoul(match.str(1));
  }else{
    throw std::runtime_error("failed to match number on line: \"" + line + '"' );
  }
}

void InputParser::split(const std::string &s, char delim, std::vector<std::string> &elems) {
  std::stringstream ss;
  ss.str(s);
  std::string item;
  while (std::getline(ss, item, delim)) {
    elems.push_back(item);
  }
}

std::vector<std::string> InputParser::split(const std::string &s, char delim) {
  std::vector<std::string> elems;
  split(s, delim, elems);
  return elems;
}
