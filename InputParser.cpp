//
// Created by Kyle Bolton on 10/7/16.
//

#include "InputParser.h"

void InputParser::parseInput() {
  int num_resources = parseNumResources();
  int num_processes = parseNumProcesses();
  unique_ptr<vector<int> > resource_instances = parseInstancesOfResources(num_resources);
  unique_ptr<vector<vector<int> > > demands = parseDemands();
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

unique_ptr<vector<vector<int> > > InputParser::parseDemands() {
  auto demands = make_unique <vector<vector<int> > >();
  //max[1,1]=7
  regex demand_parser("max\\[(\\d+),(\\d+)\\]=(\\d+)");
  string line;
  getline(input, line);
  while(regex_search(line, match, demand_parser)){
    //TODO
    getline(input, line);
  }
  return demands;
}

int InputParser::matchBareNum(const string& line) {
  regex bare_num_parser("^(\\d+)");
  if(regex_search(line, match, bare_num_parser)){
    return stoi(match[0]);
  }else{
    throw runtime_error("failed to match number on line: \"" + line + '"' );
  }
}
