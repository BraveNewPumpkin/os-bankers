//
// Created by Kyle Bolton on 10/6/16.
//

#include "Process.h"

int Process::run() {
//  pid = fork();

  pid = 0;
  if(pid == -1){
    throw runtime_error("failed to fork: " + string(strerror(errno)));
  }
  if(pid != 0){
    inter_com->registerAsParent();
  }else{
    inter_com->registerAsChild();
    setPid(pid);
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

bool Process::calculate(const unsigned int &ticks) {
//TODO
  inter_com->tellParent("calculate(" + to_string(ticks) + ")");
  //return true if successfully ran as reported by parent and false otherwise
  bool wasRun = true;
  if(wasRun){
    processing_time -= ticks;
    return true;
  }
  return false;
}

bool Process::useresources(const unsigned int &ticks) {
//TODO
  inter_com->tellParent("useresources(" + to_string(ticks) + ")");
  //return true if successfully ran as reported by parent and false otherwise
  bool wasRun = true;
  if(wasRun){
    processing_time -= ticks;
    return true;
  }
  return false;
}

bool Process::request(vector<unsigned int> requested_resources) {
  ostringstream stream;
  copy(requested_resources.begin(), requested_resources.end()-1, ostream_iterator<unsigned int>(stream, ","));
  stream << requested_resources.back();
  string resources_string = stream.str();
  inter_com->tellParent("request(" + resources_string + ")");
  //return true if successfully ran as reported by parent and false otherwise
  bool wasRun = true;
  if(wasRun){
    //reduce computation time by 1 if successful
    processing_time--;
    return true;
  }
  return false;
}

bool Process::release(vector<unsigned int> requested_resources) {
//TODO
  ostringstream stream;
  copy(requested_resources.begin(), requested_resources.end(), ostream_iterator<unsigned int>(stream, ","));
  stream << requested_resources.back();
  string resources_string = stream.str();
  resources_string.erase(resources_string.length()-1);
  inter_com->tellParent("release(" + resources_string + ")");
  //return true if successfully ran as reported by parent and false otherwise
  bool wasRun = true;
  if(wasRun){
    //reduce computation time by 1 if successful
    processing_time--;
    return true;
  }
  return false;
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

void Process::pushInstruction(Instruction instruction, vector<unsigned int> requested_resources) {
  function<bool()> delegate;
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

shared_ptr<InterCom> Process::getInterCom() {
  return inter_com;
}
