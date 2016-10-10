//
// Created by Kyle Bolton on 10/7/16.
//

#include "InputParser.h"

void InputParser::parseInput() {
  auto num_resources = parseNumResources();
  auto num_processes = parseNumProcesses();
  unique_ptr<vector<unsigned int> > resource_instances = parseInstancesOfResources(num_resources);
  unique_ptr<vector<vector<unsigned int> > > demands = parseDemands(num_resources, num_processes);
  unique_ptr<vector<unique_ptr<Process> > > processes = parseProcesses(num_processes);
  int deleteme = 1;
}

unsigned int InputParser::parseNumResources() {
  string line;
  getline(input, line);
  return matchBareNum(line);
}

unsigned int InputParser::parseNumProcesses() {
  string line;
  getline(input, line);
  return matchBareNum(line);
}

unique_ptr<vector<unsigned int> > InputParser::parseInstancesOfResources(const unsigned int& num_resources) {
  auto resource_instances = make_unique<vector<unsigned int> >();
  for (int i = 0; i < num_resources; ++i) {
    string line;
    getline(input, line);
    resource_instances->push_back(matchBareNum(line));
  }
  return resource_instances;
}

unique_ptr<vector<vector<unsigned int> > > InputParser::parseDemands(const unsigned int& num_resources, const unsigned int& num_processes) {
  auto demands = make_unique <vector<vector<unsigned int> > >(num_resources, vector<unsigned int>(num_processes, 0));
  regex demand_parser("max\\[(\\d+),(\\d+)\\]=(\\d+)");
  string line;
  getline(input, line);
  while(regex_search(line, match, demand_parser)){
    auto process_id = stoul(match.str(1))-1;
    auto resource_id = stoul(match.str(2))-1;
    auto demand = stoul(match.str(3));
    demands->at(resource_id).at(process_id) = (unsigned int)demand;
    getline(input, line);
  }
  return demands;
}

unique_ptr<vector<unique_ptr<Process> > > InputParser::parseProcesses(const unsigned int& num_processes) {
  auto processes = make_unique <vector<unique_ptr<Process> > >();
  for (int i = 0; i < num_processes; ++i) {
    processes->push_back(parseProcess());
  }
  return processes;
}

unique_ptr<Process> InputParser::parseProcess() {
  unique_ptr<Process> process = make_unique <Process>();
  regex calculate_parser("calculate\\((\\d+)\\)");
  regex useresources_parser("useresources\\((\\d+)\\)");
  regex request_parser("request\\((?:(\\d+),?)+\\)");
  regex release_parser("release\\((?:(\\d+),?)+\\)");
  regex end_parser("end");
  string line;
  bool at_end = false;
  //throw away process label
  getline(input, line);
  //get deadline and computation time
  getline(input, line);
  process->setDeadline(matchBareNum(line));
  getline(input, line);
  process->setProcessingTime(matchBareNum(line));
  //parse all instructions
  while(getline(input, line) && !at_end){
    if(regex_search(line, match, calculate_parser)){
      const unsigned int duration = stoul(match.str(1));
      process->pushInstruction(Process::Instruction::calculate, duration);
    }else if(regex_search(line, match, useresources_parser)){
      const unsigned int duration = stoul(match.str(1));
      process->pushInstruction(Process::Instruction::useresources, duration);
    }else if(regex_search(line, match, request_parser)){
      auto requested_resources = make_unique<vector<const unsigned int> >();
      const string& submatch = *(match.begin()+1);
      //TODO fix
//      for_each(match.begin()+1, match.end(), [&requested_resources](const string& sub_match){
//        unsigned int resource_instances = stoul(sub_match);
//        requested_resources->push_back(resource_instances);
//      });
//      process->pushInstruction(Process::Instruction::request, requested_resources);
    }else if(regex_search(line, match, release_parser)){
      auto requested_resources = make_unique<vector<const unsigned int> >();
//      for_each(match.begin()+1, match.end(), [&requested_resources](const string& sub_match){
//        unsigned int resource_instances = stoul(sub_match);
//        requested_resources->push_back(resource_instances);
//      });
//      process->pushInstruction(Process::Instruction::release, requested_resources);
    }else if(regex_search(line, match, end_parser)){
      at_end = true;
    }
  }
  process->pushInstruction(Process::Instruction::calculate, 2);
  return process;
}

unsigned int InputParser::matchBareNum(const string& line) {
  regex bare_num_parser("^(\\d+)");
  if(regex_search(line, match, bare_num_parser)){
    return (unsigned int)stoul(match.str(1));
  }else{
    throw runtime_error("failed to match number on line: \"" + line + '"' );
  }
}
