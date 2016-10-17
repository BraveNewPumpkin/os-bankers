//
// Created by Kyle Bolton on 10/13/16.
//

#include "EdfSjfScheduler.h"

function<bool(unique_ptr<Process> &a, unique_ptr<Process> &b)> EdfSjfScheduler::makeComparator(){
  return [](unique_ptr<Process> &a, unique_ptr<Process> &b)->bool {
    if (a->getDeadline() < b->getDeadline()) {
      return true;
    } else if (a->getDeadline() > b->getDeadline()) {
      return false;
    } else {
      return a->getProcessingTime() < b->getProcessingTime();
    }
  };
}

void EdfSjfScheduler::processBlocked() {
  blocked_process_indices.insert(*unfinished_process_iterator);
  if (blocked_process_indices.size() >= unfinished_process_indices.size()) { //TODO check to see if this works
    throw runtime_error("DEADLOCK! All remaining processes are blocked");
  }
}

void EdfSjfScheduler::processRan() {
  blocked_process_indices.clear();
}

unique_ptr<Process>& EdfSjfScheduler::getProcessToRun() {
  //pick next process (not = previous_process, not finished process, by deadline, then computation time left)
  while(true) {
    while (unfinished_process_iterator != unfinished_process_indices.end()) {
      const unsigned int& index = *unfinished_process_iterator;
      auto processing_time = processes->at(index)->getProcessingTime();
      processing_time->lockNum();
      bool is_unfinished = (processing_time->num > 0);
      processing_time->unlockNum();
      if (is_unfinished) {
        ++unfinished_process_iterator;
        return processes->at(index);
      } else {
        //remove finished process from the list. iterator points to next element.
        unfinished_process_iterator = unfinished_process_indices.erase(unfinished_process_iterator);
      }
    }
    unfinished_process_iterator = unfinished_process_indices.begin();
  }
}

unique_ptr<vector<unsigned int>> EdfSjfScheduler::getDeadlinesPassed(unsigned int& clock) {
  auto passed_deadlines = make_unique<vector<unsigned int> >();
  //check deadlines starting at last_passed_deadline_index until not passed
  while(next_deadline_iterator != processes->end() && (*next_deadline_iterator)->getDeadline() < clock){
    passed_deadlines->emplace_back((*next_deadline_iterator)->getPid());
    //set new last_passed_deadline_index
    ++next_deadline_iterator;
  }
  return passed_deadlines;
}

bool EdfSjfScheduler::allProcessesFinished() {
  return unfinished_process_indices.empty();
}