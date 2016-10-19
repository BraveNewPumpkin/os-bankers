//
// Created by Kyle Bolton on 10/5/16.
//

#include <iostream>
#include <regex> //for parsing and capturing intput
#include <fstream> //for opening & reading file
#include <vector>
#include <memory> //smart pointers plz
#include <string>
#include <csignal>
#include <unistd.h> //for unix things

#include "InputParser.h"
#include "EdfSjfScheduler.h"
#include "EdfLjfScheduler.h"
//#include "Bank.h"
//#include "Process.h"

using namespace std;


int main(int argc, char* argv[]){
  try {
    //don't crash on SIGPIPE, let's handle it with exceptions like civilized persons
    signal(SIGPIPE, SIG_IGN);
    string program_name = argv[0];
    //define closure for printing out usage
    auto printUsage = [&program_name]() {
      cout << "usage: " << program_name << " SJF|LJF /path/to/input/file.txt" << endl;
    };
    //check number of parameters
    if (argc < 3) {
      cerr << "must pass path to input file as 2nd argument" << endl;
      printUsage();
      return EXIT_FAILURE;
    }
    string tie_breaking_strategy = argv[1];
    string input_filepath = argv[2];
    //check if asking for help
    regex help_arg("^-{1,2}[Hh](?:elp)?$");
    if (regex_match(tie_breaking_strategy, help_arg) || regex_match(input_filepath, help_arg)) {
      printUsage();
      return EXIT_SUCCESS;
    }
    //attempt to open input file
    ifstream input_stream(input_filepath);
    if (!input_stream.is_open()) {
      string message = "failed to open input file: \"" + input_filepath + "\"";
      throw runtime_error(message);
    }
    unique_ptr<Bank> bank;
    unique_ptr<vector<unique_ptr<Process> > > processes;
    //parse file
    InputParser input_parser(input_stream);
    input_parser.parseInput(bank, processes);
    //explicitly close file before forking
    input_stream.close();
    //run processes
    for(unique_ptr<Process>& process: *processes){
      if(process->run() == 0){
        //am child so exit
        return EXIT_SUCCESS;
      }
    }
    unique_ptr<EdfScheduler> scheduler;
    if(tie_breaking_strategy == "SJF") {
      scheduler = make_unique<EdfSjfScheduler>(processes);
    } else if (tie_breaking_strategy == "LJF"){
      scheduler = make_unique<EdfLjfScheduler>(processes);
    } else{
      cerr << "must pass tie breaking strategy of SJF or LJF as 1rst argument" << endl;
      printUsage();
      return EXIT_FAILURE;
    }
    bool all_done = false;
    unsigned int clock = 0;

    regex instruction_regex("(\\w+)\\s*");
    smatch matches;

    while(!all_done){
      unique_ptr<Process>& process = scheduler->getProcessToRun(bank);
      auto inter_com = process->getInterCom();
      inter_com->tellChild("run");
      unique_ptr<string> response = inter_com->listenToChild();
      cout << "instruction from child (" << string(*process) << "): " << *response;
      regex_search(*response, matches, instruction_regex);
      string instruction_string(matches[1].str());
      Process::Instruction instruction = process->stringToInstruction(instruction_string);
      vector<unsigned int> args;
      string::const_iterator search_start(response->cbegin() + matches.position() + matches.length());
      while(regex_search(search_start, response->cend(), matches, instruction_regex)){
        args.push_back((unsigned int)stoul(matches[1]));
        search_start += matches.position() + matches.length();
      }
      if(bank->requestApproval(process, instruction, args)){
        inter_com->tellChild("success");
        cout << " was successful" << endl;
        response = inter_com->listenToChild();
        unsigned int clicks_elapsed = (unsigned int)stoul(*response);
        clock += clicks_elapsed;
        process->setProcessingTime(process->getProcessingTime() - clicks_elapsed);
        scheduler->processRan();
      }else{
        inter_com->tellChild("failure");
        cout << " failed" << endl;
        scheduler->processBlocked();
        clock++;
      }
      //check deadlines & report any newly passed ones
      unique_ptr<vector<unsigned int> > passed_deadlines = scheduler->getDeadlinesPassed(clock);
      for(unsigned int passed_deadline: *passed_deadlines){
        cout << "deadline passed for id: " + to_string(passed_deadline) << endl;
      }
      all_done = scheduler->allProcessesFinished();
    }
  }catch (const regex_error& e){
    cerr << "unhandled std::regex_error caught in main: " << e.what() << " code: " << e.code()<< endl;
    return EXIT_FAILURE;
  }catch (const exception& e){
    cerr << "unhandled std::exception caught in main: " << e.what() << endl;
    return EXIT_FAILURE;
  }catch (...){
    cerr << "unhandled exception of unknown type caught in main: " << endl;
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

