//
// Created by Kyle Bolton on 10/6/16.
//

#ifndef TEST_PROCESS_H
#define TEST_PROCESS_H

#include <vector>
#include <functional>
#include <utility>
#include <memory>
#include <cstring>
#include <cerrno>
#include <unistd.h>

#include "InterCom.h"

using namespace std;

class Process {
private:
  InterCom inter_com;
  unsigned int pid;
  unsigned int deadline;
  unsigned int processing_time;
  vector<function<void()> > instructions;

  void calculate(const unsigned int& ticks);
  void useresources(const unsigned int& ticks);
  void request(unique_ptr<vector<unsigned int> >& requested_resources);
  void release(unique_ptr<vector<unsigned int> >& requested_resources);
public:
  int run();

  enum class Instruction {calculate, useresources, request, release};
  //typedef void (Process::*calculate)(const unsigned int&);

  void pushInstruction(Instruction instruction, const unsigned int& ticks);
  void pushInstruction(Instruction instruction, unique_ptr<vector<unsigned int> >& requested_resources);

  unsigned int getPid() const;
  void setPid(const unsigned int pid);
  unsigned int getDeadline() const;
  void setDeadline(const unsigned int deadline);
  unsigned int getProcessingTime() const;
  void setProcessingTime(const unsigned int processing_time);
};


#endif //TEST_PROCESS_H
