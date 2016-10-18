//
// Created by Kyle Bolton on 10/6/16.
// hold information about "resources"
//

#ifndef TEST_BANK_H
#define TEST_BANK_H

#include <vector>
#include "Resource.h"
#include "Process.h"


class Bank {
private:
  std::vector<std::unique_ptr<Resource> > resources;
  std::unique_ptr<std::vector<std::vector<unsigned int> > > claims;
  std::unique_ptr<std::vector<std::vector<unsigned int> > > debits;
public:
  Bank(const unsigned int& num_processes, const unsigned int& num_resources, std::unique_ptr<std::vector<std::vector<unsigned int> > >& claims):
   claims(std::move(claims)),
   debits(std::make_unique<std::vector<std::vector<unsigned int> > >(num_processes, std::vector<unsigned int>(num_resources, 0))) {};
  //creates and adds a resource with given number of instances and returns the id
  const unsigned int addResource(const unsigned int& num_instances);
  bool releaseResources(const std::unique_ptr<Process>& process, std::vector<unsigned int> resources);
  void releaseAllResourcesForProcess(const std::unique_ptr<Process>& process);
  std::unique_ptr<Resource>& getResource(const int& resource_id);

  bool requestApproval(const std::unique_ptr<Process>& process, const Process::Instruction& instruction, const std::vector<unsigned int>& args);
  bool bankers(const std::unique_ptr<Process>& process, std::vector<unsigned int> requested_resources);



};


#endif //TEST_BANK_H
