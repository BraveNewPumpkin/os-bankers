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
    setPid(pid);
    for(auto instruction_iterator = instructions.begin();
        instruction_iterator != instructions.end();
        ++instruction_iterator){
      //read from pipe (wait)
      inter_com.listenToParent();
      //invoke(this, instruction); c++17 only
      if(!(*instruction_iterator)()){
        //if instruction was not run, move iterator back
        instruction_iterator--;
      }
    }
  }
  return pid;
}

bool Process::calculate(const unsigned int &ticks) {
//TODO
}

bool Process::useresources(const unsigned int &ticks) {
//TODO
}

bool Process::request(unique_ptr<vector<unsigned int> > &requested_resources) {
//TODO
  //reduce computation time by 1 if successful
}

bool Process::release(unique_ptr<vector<unsigned int> > &requested_resources) {
//TODO
  //reduce computation time by 1 if successful
}

void Process::pushInstruction(Instruction instruction, const unsigned int& ticks) {
  //__bind<__mem_fn<void (Process::*)(const unsigned int&)>, Args& > temp = bind(mem_fn(&Process::calculate), forward<Args>(args));
  function<bool()> delegate;
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
      throw runtime_error("invalid arguments for Instruction");
  }
  instructions.push_back(delegate);
}

void Process::pushInstruction(Instruction instruction, unique_ptr<vector<unsigned int> >& requested_resources) {
  function<bool()> delegate;
  switch(instruction){
    case Instruction::request:
      delegate = [&]()->bool{
        return request(requested_resources);
      };
      break;
    case Instruction::release:
      delegate = [&]()->bool{
        return release(requested_resources);
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
