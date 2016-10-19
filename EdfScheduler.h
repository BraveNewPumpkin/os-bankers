//
// Created by Kyle Bolton on 10/18/16.
//

#ifndef OS_BANKERS_EDFSCHEDULER_H
#define OS_BANKERS_EDFSCHEDULER_H

#include <list>
#include <unordered_set>
#include <numeric>

#include "Process.h"
#include "Bank.h"
#include "ProcessScheduler.h"

class EdfScheduler : public ProcessScheduler{
private:
  std::unordered_set<unsigned int> blocked_process_indices;
  std::list<unsigned int> unfinished_process_indices;
  std::vector<std::unique_ptr<Process> >::iterator next_deadline_iterator;
  std::list<unsigned int>::iterator unfinished_process_iterator;
protected:
  std::function<bool(std::unique_ptr<Process> &a, std::unique_ptr<Process> &b)> makeComparator();
public:
  EdfScheduler(std::unique_ptr<std::vector<std::unique_ptr<Process> > > &processes) :
  unfinished_process_indices(processes->size()),
  unfinished_process_iterator(unfinished_process_indices.begin()) {
    this->processes = std::make_unique<std::vector<std::unique_ptr<Process> > >();
    //move vector contents
    for(std::unique_ptr<Process>& process: (*processes)){
      this->processes->emplace_back(std::move(process));
    }
    //sort processes to accomplish EDF
    std::sort(this->processes->begin(), this->processes->end(), makeComparator());
    //populate unfinished index list
    std::iota(unfinished_process_indices.begin(), unfinished_process_indices.end(), 0);
    next_deadline_iterator = this->processes->begin();
  };

  std::unique_ptr<std::vector<unsigned int>> getDeadlinesPassed(unsigned int& clock);
  std::unique_ptr<Process>& getProcessToRun(std::unique_ptr<Bank>& bank);
  void processBlocked();
  void processRan();

  bool allProcessesFinished();
};


#endif //OS_BANKERS_EDFSCHEDULER_H
