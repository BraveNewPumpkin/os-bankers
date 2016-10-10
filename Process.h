//
// Created by Kyle Bolton on 10/6/16.
//

#ifndef TEST_PROCESS_H
#define TEST_PROCESS_H

#include <vector>
#include <functional>
#include <utility>

using namespace std;

class Process {
private:
  unsigned int pid;
  unsigned int deadline;
  vector<function<void()> > instructions;

  void calculate(const unsigned int& ticks);
  void useresources(const unsigned int& ticks);
  void request(unique_ptr<vector<const unsigned int> > requested_resources);
  void release(unique_ptr<vector<const unsigned int> > requested_resources);
public:
  void run();

  enum class Instruction {calculate, useresources, request, release};
  //typedef void (Process::*calculate)(const unsigned int&);

  template<typename... Args>
  void pushInstruction(Instruction instruction, Args&&... args){
    //__bind<__mem_fn<void (Process::*)(const unsigned int&)>, Args& > temp = bind(mem_fn(&Process::calculate), forward<Args>(args));
    function<void()> delegate;
    switch(instruction){
      case Instruction::calculate:
        delegate = [&](){
          calculate(forward<Args>(args)...);
        };
      break;
      case Instruction::useresources:
        delegate = [&](){
          useresources(forward<Args>(args)...);
        };
      break;
      case Instruction::request:
        delegate = [&](){
          request(forward<Args>(args)...);
        };
      break;
      case Instruction::release:
        delegate = [&](){
          release(forward<Args>(args)...);
        };
      break;
    }
    instructions.push_back(delegate);
  }

};


#endif //TEST_PROCESS_H
