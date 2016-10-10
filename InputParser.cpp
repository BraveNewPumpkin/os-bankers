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
  //TODO
  unique_ptr<Process> process = make_unique <Process>();
  process->pushInstruction<const unsigned int&>(Process::Instruction::calculate, 2);
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
