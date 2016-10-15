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
  list::iterator remaining_unfinished_process_iterator = unfinished_process_iterator;
  for_each(
     remaining_unfinished_process_iterator,
     unfinished_process_indices.end(),
     [&blocked_process_indicies](const unsigned int& index){
       if(blocked_process_indicies.count(index) > 0){
         throw runtime_error("DEADLOCK! All remaining processes are blocked");
       }
     }
  );
}

void EdfSjfScheduler::processRan() {
  blocked_process_indicies.clear();
  last_run_index = *unfinished_process_iterator;
  unfinished_process_iterator = unfinished_process_indices.begin();
}

unique_ptr<Process>& EdfSjfScheduler::getProcessToRun() {
  //pick next process (not = previous_process, not finished process, by deadline, then computation time left)
  for(; unfinished_process_iterator != unfinished_process_indices.end(); ++unfinished_process_iterator){
    const unsigned int& index = *unfinished_process_iterator;
    if(index != last_run_index){
      if(processes->at(index)->getProcessingTime() > 0){
        return processes->at(index);
      }else{
        //remove finished process from the list
        unfinished_process_indices.erase(unfinished_process_iterator);
        unfinished_process_iterator = unfinished_process_indices.begin();
      }
    }
  }
}

unique_ptr<vector<unsigned int>> EdfSjfScheduler::getDeadlinesPassed(unsigned int& clock) {
  auto passed_deadlines = make_unique<vector<unsigned int> >();
  //TODO
  //check deadlines starting at last_passed_deadline_index until not passed
  //push onto vector
  //set new last_passed_deadline_index
  return passed_deadlines;
}

bool EdfSjfScheduler::allProcessesFinished() {
  return unfinished_process_indices.empty();
}