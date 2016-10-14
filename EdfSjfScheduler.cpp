//
// Created by Kyle Bolton on 10/13/16.
//

#include "EdfSjfScheduler.h"

unique_ptr<Process>& EdfSjfScheduler::getProcessToRun() {
  //TODO
  //pick next process (not = previous_process, not finished process, by deadline, then computation time left)
  return (*processes)[0];
}

vector<unsigned int> EdfSjfScheduler::getDeadlinesPassed(unsigned int& clock) {
  //TODO
}

bool EdfSjfScheduler::allProcessesFinished() {
  return unfinished_process_indices.empty();
}