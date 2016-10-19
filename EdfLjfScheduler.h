//
// Created by Kyle Bolton on 10/18/16.
//

#ifndef OS_BANKERS_EDFLJFSCHEDULER_H
#define OS_BANKERS_EDFLJFSCHEDULER_H

#include "EdfScheduler.h"

class EdfLjfScheduler : public EdfScheduler {
private:

protected:
  std::function<bool(std::unique_ptr<Process> &a, std::unique_ptr<Process> &b)> makeComparator();

public:
  EdfLjfScheduler(std::unique_ptr<std::vector<std::unique_ptr<Process> > > &processes) : EdfScheduler(processes) {}

};

#endif //OS_BANKERS_EDFLJFSCHEDULER_H
