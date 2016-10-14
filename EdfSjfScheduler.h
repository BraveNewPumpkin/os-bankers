//
// Created by Kyle Bolton on 10/13/16.
// implements ProcessScheduler interface

#ifndef TEST_EDFSJFSCHEDULER_H
#define TEST_EDFSJFSCHEDULER_H

#include <forward_list>

#include "ProcessScheduler.h"
#include "Process.h"


class EdfSjfScheduler : public ProcessScheduler {
private:
  unsigned int last_run_index;
  unsigned int last_passed_deadline_index;
  forward_list<unsigned int> unfinished_process_indices;
public:
  EdfSjfScheduler(unique_ptr<vector<unique_ptr<Process> > > &processes) :
     ProcessScheduler(processes),
     unfinished_process_indices(processes->begin(), processes->end(), 0) {}; //TODO check if this works

  vector<unsigned int> getDeadlinesPassed(unsigned int& clock);
  unique_ptr<Process>& getProcessToRun();
  bool allProcessesFinished();
};


#endif //TEST_EDFSJFSCHEDULER_H
