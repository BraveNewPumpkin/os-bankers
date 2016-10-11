//
// Created by Kyle Bolton on 10/6/16.
// hold information about "resources"
//

#ifndef TEST_BANK_H
#define TEST_BANK_H

//forward declaration of dependencies
class Resource;

#include <vector>
#include "Resource.h"

using namespace std;

class Bank {
private:
  vector<unique_ptr<Resource> > resources;
  unique_ptr<vector<vector<unsigned int> > > claims;
public:
  Bank(unique_ptr<vector<vector<unsigned int> > >& claims): claims(move(claims)) {};
  //creates and adds a resource with given number of instances and returns the id
  const int addResource(const int& num_instances);
  //sets the max demand for a given resource by a given pid
  void addClaim(const int& pid, const int& resource_id);

  unique_ptr<Resource>& getResource(const int& resource_id);

};


#endif //TEST_BANK_H
