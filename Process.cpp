//
// Created by Kyle Bolton on 10/6/16.
//

#include "Process.h"

int Process::run() {
  pid = fork();

  if(pid == -1){
    throw std::runtime_error("failed to fork: " + std::string(strerror(errno)));
  }
  if(pid != 0){
    inter_com->registerAsParent();
  }else{
    inter_com->registerAsChild();
    for(auto instruction_iterator = instructions.begin();
        instruction_iterator != instructions.end();
        ++instruction_iterator){
      //read from pipe (wait)
      inter_com->listenToParent();
      if(!(*instruction_iterator)()){
        //if instruction was not run, move iterator back
        instruction_iterator--;
      }
    }
  }
  return pid;
}


Process::operator std::string() const {
  return "pid: " + std::to_string(pid) + " id: " + std::to_string(id) + " index: " + std::to_string(index) + " deadline: " + std::to_string(deadline) + " remaining processing_time: " + std::to_string(processing_time);
}

bool Process::calculate(const unsigned int &ticks) {
  return processingActions("calculate", ticks);
}

bool Process::useresources(const unsigned int &ticks) {
  return processingActions("useresources", ticks);
}

bool Process::request(std::vector<unsigned int> requested_resources) {
  return resourceActions("request", requested_resources);
}

bool Process::release(std::vector<unsigned int> requested_resources) {
  return resourceActions("release", requested_resources);
}

bool Process::processingActions(const std::string &name, const unsigned int &ticks) {
  inter_com->tellParent(name + " " + std::to_string(ticks));
  //return true if successfully ran as reported by parent and false otherwise
  std::unique_ptr<std::string> result = inter_com->listenToParent();
  bool wasRun = (*result) == "success";
  if(wasRun){
    processing_time -= ticks;
    //tell parent how much time it took
    inter_com->tellParent(std::to_string(ticks));
  }
  return wasRun;
}

bool Process::resourceActions(const std::string& name, std::vector<unsigned int>& requested_resources){
  //TODO put join in here
  inter_com->tellParent(name + " " + resources_string);
  std::unique_ptr<std::string> result = inter_com->listenToParent();
  //return true if successfully ran as reported by parent and false otherwise
  bool wasRun = (*result) == "success";
  if(wasRun){
    //reduce computation time by 1 if successful
    processing_time--;
    //tell parent how much time it took
    inter_com->tellParent(std::to_string(1));
  }
  return wasRun;
}

void Process::pushInstruction(Instruction instruction, const unsigned int& ticks) {
  //__bind<__mem_fn<void (Process::*)(const unsigned int&)>, Args& > temp = bind(mem_fn(&Process::calculate), forward<Args>(args));
  std::function<bool()> delegate;
  switch(instruction){
    case Instruction::calculate:
      delegate = [&]()->bool{
        return calculate(ticks);
      };
    break;
    case Instruction::useresources:
      delegate = [&]()->bool{
        return useresources(ticks);
      };
    break;
    case Instruction::request:
    case Instruction::release:
      throw std::runtime_error("invalid arguments for Instruction");
  }
  instructions.push_back(delegate);
}

void Process::pushInstruction(Instruction instruction, std::vector<unsigned int> requested_resources) {
  std::function<bool()> delegate;
  switch(instruction){
    case Instruction::request:
      delegate = [this, requested_resources]()->bool{
        return request(requested_resources);
      };
      break;
    case Instruction::release:
      delegate = [this, requested_resources]()->bool{
        return release(requested_resources);
      };
      break;
    case Instruction::calculate:
    case Instruction::useresources:
      throw std::runtime_error("invalid arguments for Instruction");
  }
  instructions.push_back(delegate);
}

std::unique_ptr<std::string> Process::instructionsToString(Instruction instruction) {
  switch(instruction){
    case Instruction::calculate:
      return std::make_unique<std::string>("calculate");
    case Instruction::useresources:
      return std::make_unique<std::string>("useresources");
    case Instruction::request:
      return std::make_unique<std::string>("request");
    case Instruction::release:
      return std::make_unique<std::string>("release");
  }
}

Process::Instruction Process::stringToInstruction(const std::string& name) {
  std::regex calculate_regex("calculate");
  std::regex useresources_regex("useresources");
  std::regex request_regex("request");
  std::regex release_regex("release");

  Instruction to_return;

  if(std::regex_match(name, calculate_regex)) {
    to_return = Instruction::calculate;
  }else if(std::regex_match(name, useresources_regex)){
    to_return = Instruction::useresources;
  }else if(std::regex_match(name, request_regex)){
    to_return = Instruction::request;
  }else if(std::regex_match(name, release_regex)){
    to_return = Instruction::release;
  }
  return to_return;
}

unsigned int Process::getId() const {
  return id;
}

void Process::setId(const unsigned int& id) {
  Process::id = id;
}

unsigned int Process::getPid() const {
  return pid;
}

void Process::setPid(const unsigned int pid) {
  Process::pid = pid;
}

unsigned int Process::getDeadline() const {
  return deadline;
}

void Process::setDeadline(const unsigned int deadline) {
  Process::deadline = deadline;
}

unsigned int Process::getProcessingTime() const {
  return processing_time;
}

void Process::setProcessingTime(const unsigned int processing_time) {
  Process::processing_time = processing_time;
}

std::shared_ptr<InterCom> Process::getInterCom() {
  return inter_com;
}

unsigned int Process::getIndex() const {
  return index;
}
