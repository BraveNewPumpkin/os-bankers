//
// Created by Kyle Bolton on 10/13/16.
// implements ProcessScheduler interface

#ifndef TEST_EDFSJFSCHEDULER_H
#define TEST_EDFSJFSCHEDULER_H

#include <list>
#include <unordered_set>
#include <numeric>

#include "ProcessScheduler.h"
#include "Process.h"


class EdfSjfScheduler : public ProcessScheduler {
private:
  bool first_run;
  unsigned int last_run_index;
  vector::iterator last_passed_deadline_iterator;
  list::iterator unfinished_process_iterator;
  unordered_set<unsigned int> blocked_process_indicies;
  list<unsigned int> unfinished_process_indices;
  bool processComparator(unique_ptr<Process>& a, unique_ptr<Process>& b);
public:
  EdfSjfScheduler(unique_ptr<vector<unique_ptr<Process> > > &processes) :
     first_run(true),
     last_passed_deadline_iterator(processes->begin()),
     unfinished_process_indices(processes->size()),
     unfinished_process_iterator(unfinished_process_indices.begin()) {
    this->processes = make_unique<vector<unique_ptr<Process> > >();
    //move vector contents
    for(unique_ptr<Process>& process: (*processes)){
      this->processes->emplace_back(move(process));
    }
    //sort processes to accomplish EDF
    sort(this->processes->begin(), this->processes->end(), processComparator);
    //populate unfinished index list
    iota(unfinished_process_indices.begin(), unfinished_process_indices.end(), 0);
  };

  unique_ptr<vector<unsigned int>> getDeadlinesPassed(unsigned int& clock);
  unique_ptr<Process>& getProcessToRun();
  void processBlocked();
  void processRan();

  bool allProcessesFinished();
};


#endif //TEST_EDFSJFSCHEDULER_H
