//
// Created by Kyle Bolton on 10/6/16.
// hold information about "resources"
//

#ifndef TEST_BANK_H
#define TEST_BANK_H

//forward declaration of dependencies
class Resource;

#include "Resource.h"

class Bank {
private:
  //TODO make pointer to Resource and remove include?
  std::vector<Resource> resources;
public:
  Bank();
  //creates and adds a resource with given number of instances and returns the id
  const int addResource(const int num_instances);
  //sets the max demand for a given resource by a given pid
  void addClaim(const int pid, const int resource_id);

  Resource& getResource(const int resource_id);

};


#endif //TEST_BANK_H
