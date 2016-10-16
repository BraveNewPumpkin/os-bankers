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
  unordered_set<unsigned int> blocked_process_indices;
  list<unsigned int> unfinished_process_indices;
  vector<unique_ptr<Process> >::iterator next_deadline_iterator;
  list<unsigned int>::iterator unfinished_process_iterator;

  function<bool(unique_ptr<Process> &a, unique_ptr<Process> &b)> makeComparator();
public:
  EdfSjfScheduler(unique_ptr<vector<unique_ptr<Process> > > &processes) :
     unfinished_process_indices(processes->size()),
     unfinished_process_iterator(unfinished_process_indices.begin()) {
    this->processes = make_unique<vector<unique_ptr<Process> > >();
    //move vector contents
    for(unique_ptr<Process>& process: (*processes)){
      this->processes->emplace_back(move(process));
    }
    //sort processes to accomplish EDF
    sort(this->processes->begin(), this->processes->end(), makeComparator());
    //populate unfinished index list
    iota(unfinished_process_indices.begin(), unfinished_process_indices.end(), 0);
    next_deadline_iterator = this->processes->begin();
  };

  unique_ptr<vector<unsigned int>> getDeadlinesPassed(unsigned int& clock);
  unique_ptr<Process>& getProcessToRun();
  void processBlocked();
  void processRan();

  bool allProcessesFinished();
};


#endif //TEST_EDFSJFSCHEDULER_H
