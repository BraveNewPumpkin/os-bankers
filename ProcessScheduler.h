//
// Created by Kyle Bolton on 10/13/16.
// interface for classes used to schedule processes
//

#ifndef TEST_PROCESSSCHEDULER_H
#define TEST_PROCESSSCHEDULER_H

#include <memory>

#include "Process.h"

class Bank;

class ProcessScheduler {
protected:
  std::unique_ptr<std::vector<std::unique_ptr<Process> > > processes;
public:
  //pure virtual method
  virtual std::unique_ptr<Process>& getProcessToRun(std::unique_ptr<Bank>& bank) = 0;
  virtual bool allProcessesFinished() = 0;
};


#endif //TEST_PROCESSSCHEDULER_H
