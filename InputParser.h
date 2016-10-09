//
// Created by Kyle Bolton on 10/7/16.
//

#ifndef TEST_INPUTPARSER_H
#define TEST_INPUTPARSER_H

#include <fstream> //for opening & reading file
#include <regex> //for parsing and capturing intput
#include <string>

using namespace std;

//forward declarations
class Process;

class InputParser {
private:
  ifstream& input;
  smatch match; //holds matches if any found
  int matchBareNum(const string& line);
  int parseNumResources();
  int parseNumProcesses();
  unique_ptr<vector<int> > parseInstancesOfResources(const int& num_resources);
  unique_ptr<vector<vector<int> > > parseDemands();
  //vector<auto_ptr<Process> > parseProcesses();
  //auto_ptr<Process> parseProcess();

public:
  InputParser(ifstream &input): input(input) {};
  void parseInput();

};


#endif //TEST_INPUTPARSER_H
