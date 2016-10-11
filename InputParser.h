//
// Created by Kyle Bolton on 10/7/16.
//

#ifndef TEST_INPUTPARSER_H
#define TEST_INPUTPARSER_H

#include <fstream> //for opening & reading file
#include <regex> //for parsing and capturing intput
#include <array>
#include <string>
#include <sstream>

#include "Process.h"
#include "Bank.h"

using namespace std;


class InputParser {
private:
  ifstream& input;
  smatch match; //holds matches if any found
  unsigned int matchBareNum(const string& line);
  unsigned int parseNumResources();
  unsigned int parseNumProcesses();
  unique_ptr<vector<unsigned int> > parseInstancesOfResources(const unsigned int& num_resources);
  unique_ptr<vector<vector<unsigned int> > > parseDemands(const unsigned int& num_resources, const unsigned int& num_processes);
  unique_ptr<vector<unique_ptr<Process> > > parseProcesses(const unsigned int& num_processes);
  unique_ptr<Process> parseProcess();
  void split(const std::string &s, char delim, std::vector<std::string> &elems);
  std::vector<std::string> split(const std::string &s, char delim);

public:
  InputParser(ifstream &input): input(input) {};
  void parseInput(unique_ptr<Bank>& bank, unique_ptr<vector<unique_ptr<Process> > >& processes);

};


#endif //TEST_INPUTPARSER_H
