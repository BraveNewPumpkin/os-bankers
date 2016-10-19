//
// Created by Kyle Bolton on 10/13/16.
// implements ProcessScheduler interface

#ifndef TEST_EDFSJFSCHEDULER_H
#define TEST_EDFSJFSCHEDULER_H

#include "EdfScheduler.h"

class EdfSjfScheduler : public EdfScheduler {
private:

protected:
  std::function<bool(std::unique_ptr<Process> &a, std::unique_ptr<Process> &b)> makeComparator();

public:
  EdfSjfScheduler(std::unique_ptr<std::vector<std::unique_ptr<Process> > > &processes) : EdfScheduler(processes) {}

};


#endif //TEST_EDFSJFSCHEDULER_H
