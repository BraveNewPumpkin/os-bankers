//
// Created by Kyle Bolton on 10/6/16.
//

#ifndef TEST_PROCESS_H
#define TEST_PROCESS_H

#include <vector>
#include <functional>
#include <utility>
#include <memory>
#include <sstream>
#include <cstring>
#include <cerrno>
#include <unistd.h>

#include "InterCom.h"

using namespace std;

class Process {
private:
  shared_ptr<InterCom> inter_com;
  unsigned int pid;
  unsigned int deadline;
  unsigned int processing_time;
  vector<function<bool()> > instructions;

  bool calculate(const unsigned int &ticks);
  bool useresources(const unsigned int &ticks);
  bool request(vector<unsigned int> requested_resources);
  bool release(vector<unsigned int> requested_resources);
public:
  Process() : inter_com(make_shared<InterCom>()) {};
  int run();

  enum class Instruction {calculate, useresources, request, release};
  //typedef void (Process::*calculate)(const unsigned int&);

  void pushInstruction(Instruction instruction, const unsigned int& ticks);
  void pushInstruction(Instruction instruction, vector<unsigned int> requested_resources);

  unsigned int getPid() const;
  void setPid(const unsigned int pid);
  unsigned int getDeadline() const;
  void setDeadline(const unsigned int deadline);
  unsigned int getProcessingTime() const;
  void setProcessingTime(const unsigned int processing_time);
  shared_ptr<InterCom> getInterCom();

};


#endif //TEST_PROCESS_H
