//
// Created by Kyle Bolton on 10/13/16.
// interface for classes used to schedule processes
//

#ifndef TEST_PROCESSSCHEDULER_H
#define TEST_PROCESSSCHEDULER_H

#include <memory>

#include "Process.h"

using namespace std;

class ProcessScheduler {
protected:
  unique_ptr<vector<unique_ptr<Process> > > processes;
public:
  ProcessScheduler(unique_ptr<vector<unique_ptr<Process> > > &processes) : processes(move(processes)) {};
  //pure virtual method
  virtual unique_ptr<Process>& getProcessToRun() = 0;
  virtual bool allProcessesFinished() = 0;
};


#endif //TEST_PROCESSSCHEDULER_H
