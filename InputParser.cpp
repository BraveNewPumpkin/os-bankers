//
// Created by Kyle Bolton on 10/7/16.
//

#include "InputParser.h"

void InputParser::parseInput() {
  int num_resources = parseNumResources();
  int num_processes = parseNumProcesses();
  unique_ptr<vector<int> > resource_instances = parseInstancesOfResources(num_resources);
  unique_ptr<vector<vector<int> > > demands = parseDemands(num_resources, num_processes);
  unique_ptr<vector<unique_ptr<Process> > > processes = parseProcesses(num_processes);
  int deleteme = 1;
}

int InputParser::parseNumResources() {
  string line;
  getline(input, line);
  return matchBareNum(line);
}

int InputParser::parseNumProcesses() {
  string line;
  getline(input, line);
  return matchBareNum(line);
}

unique_ptr<vector<int> > InputParser::parseInstancesOfResources(const int& num_resources) {
  auto resource_instances = make_unique<vector<int> >();
  for (int i = 0; i < num_resources; ++i) {
    string line;
    getline(input, line);
    resource_instances->push_back(matchBareNum(line));
  }
  return resource_instances;
}

unique_ptr<vector<vector<int> > > InputParser::parseDemands(const int& num_resources, const int& num_processes) {
  auto demands = make_unique <vector<vector<int> > >(num_resources, vector<int>(num_processes, 0));
  regex demand_parser("max\\[(\\d+),(\\d+)\\]=(\\d+)");
  string line;
  getline(input, line);
  while(regex_search(line, match, demand_parser)){
    int resource_id = stoi(match.str(1));
    int process_id = stoi(match.str(2));
    int demand = stoi(match.str(3));
    demands->at(resource_id).at(process_id) = demand;
    getline(input, line);
  }
  return demands;
}

unique_ptr<vector<unique_ptr<Process> > > InputParser::parseProcesses(const int& num_processes) {
  auto processes = make_unique <vector<unique_ptr<Process> > >();
  for (int i = 0; i < num_processes; ++i) {
    processes->push_back(parseProcess());
  }
  return processes;
}

unique_ptr<Process> InputParser::parseProcess() {
  //TODO
}

int InputParser::matchBareNum(const string& line) {
  regex bare_num_parser("^(\\d+)");
  if(regex_search(line, match, bare_num_parser)){
    return stoi(match.str(1));
  }else{
    throw runtime_error("failed to match number on line: \"" + line + '"' );
  }
}
