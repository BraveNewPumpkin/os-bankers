//
// Created by Kyle Bolton on 10/13/16.
//

#include "EdfSjfScheduler.h"

bool EdfSjfScheduler::processComparator(unique_ptr<Process> &a, unique_ptr<Process> &b) {
  if(a->getDeadline() < b->getDeadline()){
    return true;
  }else if(a->getDeadline() > b->getDeadline()){
    return false;
  }else{
    return a->getProcessingTime() < b->getProcessingTime();
  }
}

void EdfSjfScheduler::processBlocked() {
  blocked_process_indicies.insert(*unfinished_process_iterator);
  if (blocked_process_indicies.size() >= unfinished_process_indices.size()) { //TODO check to see if this works
    if (blocked_process_indicies.count(index) > 0) {
      throw runtime_error("DEADLOCK! All remaining processes are blocked");
    }
  }
}

void EdfSjfScheduler::processRan() {
  blocked_process_indicies.clear();
  last_run_index = *unfinished_process_iterator;
}

unique_ptr<Process>& EdfSjfScheduler::getProcessToRun() {
  //pick next process (not = previous_process, not finished process, by deadline, then computation time left)
  while(true) {
    while (unfinished_process_iterator != unfinished_process_indices.end()) {
      const unsigned int& index = *unfinished_process_iterator;
//      if (!first_run && unfinished_process_indices.size() > 1 && index == last_run_index) {//TODO only throw if this one is also deadlocked unless it is last one left.
//        throw runtime_error("DEADLOCK! All remaining processes are blocked");
//      }
      if (processes->at(index)->getProcessingTime() > 0) {
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
  //TODO handle initial case and it should be looking ahead one: "LAST passed iterator"
  while(last_passed_deadline_iterator != processes->end() && processes->at(*last_passed_deadline_iterator)->getDeadline() < clock){
    passed_deadlines->emplace_back(processes->at(*last_passed_deadline_iterator)->getPid());
    //set new last_passed_deadline_index
    ++last_passed_deadline_iterator;
  }
  return passed_deadlines;
}

bool EdfSjfScheduler::allProcessesFinished() {
  return unfinished_process_indices.empty();
}