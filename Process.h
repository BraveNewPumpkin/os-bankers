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

#include "InterCom.h"

class Process {
private:
  unsigned int index;
  std::shared_ptr<InterCom> inter_com;
  unsigned int id;
  unsigned int pid;
  unsigned int deadline;
  unsigned int processing_time;
  std::vector<std::function<bool()> > instructions;

  bool calculate(const unsigned int &ticks);
  bool useresources(const unsigned int &ticks);
  bool request(std::vector<unsigned int> requested_resources);
  bool release(std::vector<unsigned int> requested_resources);

  bool resourceActions(const std::string& name, std::vector<unsigned int>& requested_resources);
  bool processingActions(const std::string& name, const unsigned int &ticks);
public:
  Process(const unsigned int& index) : index(index), inter_com(std::make_shared<InterCom>()) {};

  operator std::string() const;

  int run();

  enum class Instruction {calculate, useresources, request, release};
  std::unique_ptr<std::string> instructionsToString(Instruction instruction);
  Instruction stringToInstruction(const std::string& name);

  void pushInstruction(Instruction instruction, const unsigned int& ticks);
  void pushInstruction(Instruction instruction, std::vector<unsigned int> requested_resources);


  unsigned int getIndex() const;
  unsigned int getId() const;
  void setId(const unsigned int& id);
  unsigned int getPid() const;
  void setPid(const unsigned int pid);
  unsigned int getDeadline() const;
  void setDeadline(const unsigned int deadline);
  unsigned int getProcessingTime() const;
  void setProcessingTime(const unsigned int processing_time);
  std::shared_ptr<InterCom> getInterCom();
};


#endif //TEST_PROCESS_H
