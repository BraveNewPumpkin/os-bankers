//
// Created by Kyle Bolton on 10/6/16.
//

#ifndef TEST_PROCESS_H
#define TEST_PROCESS_H

#include <vector>
#include <regex> //for matching Instructions
#include <functional>
#include <utility>
#include <memory>
#include <sstream>
#include <cstring>
#include <cerrno>
#include <unistd.h>

#include <boost/interprocess/managed_shared_memory.hpp>

#include "InterCom.h"
#include "SyncedSharedUnsignedInt.h"

class Process {
private:
  std::shared_ptr<InterCom> inter_com;
  unsigned int pid;
  unsigned int deadline;
  unsigned int initial_processing_time;
  SyncedSharedUnsignedInt* processing_time;
  std::vector<std::function<bool()> > instructions;

  bool calculate(const unsigned int &ticks);
  bool useresources(const unsigned int &ticks);
  bool request(std::vector<unsigned int> requested_resources);
  bool release(std::vector<unsigned int> requested_resources);

  bool resourceActions(const std::string& name, std::vector<unsigned int>& requested_resources);
  bool processingActions(const std::string& name, const unsigned int &ticks);
public:
  Process() : inter_com(std::make_shared<InterCom>()) {
  }

  int run(boost::interprocess::managed_shared_memory& segment);

  enum class Instruction {calculate, useresources, request, release};
  std::unique_ptr<std::string> instructionsToString(Instruction instruction);
  Instruction stringToInstruction(const std::string& name);

  void pushInstruction(Instruction instruction, const unsigned int& ticks);
  void pushInstruction(Instruction instruction, std::vector<unsigned int> requested_resources);

  unsigned int getPid() const;
  void setPid(const unsigned int pid);
  unsigned int getDeadline() const;
  void setDeadline(const unsigned int deadline);
  SyncedSharedUnsignedInt* getProcessingTime() const;
  void setInitialProcessingTime(const unsigned int& processing_time);
  std::shared_ptr<InterCom> getInterCom();

};


#endif //TEST_PROCESS_H
