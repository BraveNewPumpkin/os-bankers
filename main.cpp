//
// Created by Kyle Bolton on 10/5/16.
//

#include <iostream>
#include <regex> //for parsing and capturing intput
#include <fstream> //for opening & reading file
#include <vector>
#include <memory> //smart pointers plz
#include <string>
#include <unistd.h> //for unix things

#include "InputParser.h"
//#include "Bank.h"
//#include "Process.h"

using namespace std;

int main(int argc, char* argv[]){
  try {

    string program_name = argv[0];
    //define closure for printing out usage
    auto printUsage = [&program_name]() {
      cout << "usage: " << program_name << " /path/to/input/file.txt" << endl;
    };
    //check number of parameters
    if (argc < 2) {
      cerr << "must pass path to input file as 1rst argument" << endl;
      printUsage();
      return EXIT_FAILURE;
    }
    string inpute_filepath = argv[1];
    //check if asking for help
    regex help_arg("^-{1,2}[Hh](?:elp)?$");
    if (regex_match(inpute_filepath, help_arg)) {
      printUsage();
      return EXIT_SUCCESS;
    }
    //attempt to open input file
    ifstream input_stream(inpute_filepath);
    if (!input_stream.is_open()) {
      string message = "failed to open input file: \"" + inpute_filepath + "\"";
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
        //am child so exit loop
        break;
      }
    }
    bool all_done = false;
    unsigned int clock = 0;
    /*
    unique_ptr<Process> previous_process;
    int passed_deadline_index = -1;
    while(!all_done){
      pick next process (not = previous_process, by deadline, then computation time left)
      write to pipe
      read from pipe
      add time to clock
      check deadlines & report any newly passed ones
      check if all procs report done
    }
     */
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

