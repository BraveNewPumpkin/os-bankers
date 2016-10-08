//
// Created by Kyle Bolton on 10/5/16.
//

#include <iostream>
#include <regex> //for parsing and capturing intput
#include <fstream> //for opening & reading file
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
    //parse file
    // Bank bank;
    InputParser input_parser(input_stream);
    input_parser.parseInput();
    //explicitly close file before forking
    input_stream.close();
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
