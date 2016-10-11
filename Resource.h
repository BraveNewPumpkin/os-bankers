//
// Created by Kyle Bolton on 10/6/16.
//

#ifndef TEST_RESOURCE_H
#define TEST_RESOURCE_H

#include <string>

using namespace std;

class Resource {
private:
  unsigned int available;
  unsigned int used;
public:
  Resource(const int& num_instances);
  unsigned int getAvailable() const;
  unsigned int getUsed() const;
  void allocate(const unsigned int& num_instances);
  void release(const unsigned int& num_instances);
};


#endif //TEST_RESOURCE_H
