//
// Created by Kyle Bolton on 10/13/16.
//

#include "EdfSjfScheduler.h"

std::function<bool(std::unique_ptr<Process> &a, std::unique_ptr<Process> &b)> EdfSjfScheduler::makeComparator(){
  return [](std::unique_ptr<Process> &a, std::unique_ptr<Process> &b)->bool {
    if (a->getDeadline() < b->getDeadline()) {
      return true;
    } else if (a->getDeadline() > b->getDeadline()) {
      return false;
    } else {
      return a->getProcessingTime() < b->getProcessingTime();
    }
  };
}
