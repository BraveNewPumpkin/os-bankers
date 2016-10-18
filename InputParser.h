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


class InputParser {
private:
  std::ifstream& input;
  std::smatch match; //holds matches if any found
  unsigned int matchBareNum(const std::string& line);
  unsigned int parseNumResources();
  unsigned int parseNumProcesses();
  std::unique_ptr<std::vector<unsigned int> > parseInstancesOfResources(const unsigned int& num_resources);
  std::unique_ptr<std::vector<std::vector<unsigned int> > > parseDemands(const unsigned int& num_processes, const unsigned int& num_resources);
  std::unique_ptr<std::vector<std::unique_ptr<Process> > > parseProcesses(const unsigned int& num_processes);
  std::unique_ptr<Process> parseProcess(const unsigned int& index);

public:
  InputParser(std::ifstream &input): input(input) {};
  void parseInput(std::unique_ptr<Bank>& bank, std::unique_ptr<std::vector<std::unique_ptr<Process> > >& processes);

};


#endif //TEST_INPUTPARSER_H
