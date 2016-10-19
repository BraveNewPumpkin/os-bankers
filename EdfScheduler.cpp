//
// Created by Kyle Bolton on 10/18/16.
//

#include "EdfScheduler.h"

std::function<bool(std::unique_ptr<Process> &a, std::unique_ptr<Process> &b)> EdfScheduler::makeComparator(){
  return [](std::unique_ptr<Process> &a, std::unique_ptr<Process> &b)->bool {
    return a < b;
  };
}

void EdfScheduler::processBlocked() {
  blocked_process_indices.insert(*unfinished_process_iterator);
  if (blocked_process_indices.size() >= unfinished_process_indices.size()) {
    throw std::runtime_error("DEADLOCK! All remaining processes are blocked");
  }
}

void EdfScheduler::processRan() {
  blocked_process_indices.clear();
}

std::unique_ptr<Process>& EdfScheduler::getProcessToRun(std::unique_ptr<Bank>& bank) {
  //pick next process (not = previous_process, not finished process, by deadline, then computation time left)
  while(true) {
    while (unfinished_process_iterator != unfinished_process_indices.end()) {
      const unsigned int& index = *unfinished_process_iterator;
      if (processes->at(index)->getProcessingTime() > 0) {
        ++unfinished_process_iterator;
        return processes->at(index);
      } else {
        //release all resources used by this process
        bank->releaseAllResourcesForProcess(processes->at(index));
        //remove finished process from the list. iterator points to next element.
        unfinished_process_iterator = unfinished_process_indices.erase(unfinished_process_iterator);
      }
    }
    unfinished_process_iterator = unfinished_process_indices.begin();
  }
}

std::unique_ptr<std::vector<unsigned int>> EdfScheduler::getDeadlinesPassed(unsigned int& clock) {
  auto passed_deadlines = std::make_unique<std::vector<unsigned int> >();
  //check deadlines starting at last_passed_deadline_index until not passed
  while(next_deadline_iterator != processes->end() && (*next_deadline_iterator)->getDeadline() < clock){
    passed_deadlines->emplace_back((*next_deadline_iterator)->getId());
    //set new last_passed_deadline_index
    ++next_deadline_iterator;
  }
  return passed_deadlines;
}

bool EdfScheduler::allProcessesFinished() {
  return unfinished_process_indices.empty();
}