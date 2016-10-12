//
// Created by Kyle Bolton on 10/6/16.
//

#include "Process.h"

int Process::run() {
  pid = fork();
  if(pid == -1){
    throw runtime_error("failed to fork: " + string(strerror(errno)));
  }
  if(pid != 0){
    inter_com.registerAsParent();
  }else{
    inter_com.registerAsChild();
    for(function<void()> instruction: instructions){
      //read from pipe (wait)
      inter_com.listenToParent();
      //invoke(this, instruction); c++17 only
      instruction();
    }
  }
  return pid;
}

void Process::calculate(const unsigned int &ticks) {
//TODO
}

void Process::useresources(const unsigned int &ticks) {
//TODO
}

void Process::request(unique_ptr<vector<unsigned int> >& requested_resources) {
//TODO
}

void Process::release(unique_ptr<vector<unsigned int> >& requested_resources) {
//TODO
}

void Process::pushInstruction(Instruction instruction, const unsigned int& ticks) {
  //__bind<__mem_fn<void (Process::*)(const unsigned int&)>, Args& > temp = bind(mem_fn(&Process::calculate), forward<Args>(args));
  function<void()> delegate;
  switch(instruction){
    case Instruction::calculate:
      delegate = [&](){
        calculate(ticks);
      };
    break;
    case Instruction::useresources:
      delegate = [&](){
        useresources(ticks);
      };
    break;
    case Instruction::request:
    case Instruction::release:
      throw runtime_error("invalid arguments for Instruction");
  }
  instructions.push_back(delegate);
}

void Process::pushInstruction(Instruction instruction, unique_ptr<vector<unsigned int> >& requested_resources) {
  function<void()> delegate;
  switch(instruction){
    case Instruction::request:
      delegate = [&](){
        request(requested_resources);
      };
      break;
    case Instruction::release:
      delegate = [&](){
        release(requested_resources);
      };
      break;
    case Instruction::calculate:
    case Instruction::useresources:
      throw runtime_error("invalid arguments for Instruction");
  }
  instructions.push_back(delegate);
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
