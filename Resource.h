//
// Created by Kyle Bolton on 10/6/16.
//

#ifndef TEST_RESOURCE_H
#define TEST_RESOURCE_H

#include <string>

class Resource {
private:
  const unsigned int index;
  unsigned int available;
  unsigned int used;
public:
  Resource(const unsigned int& index, const unsigned int& num_instances): index(index), available(num_instances), used(0) {};
  operator std::string() const;
  unsigned int getAvailable() const;
  unsigned int getUsed() const;
  void allocate(const unsigned int& num_instances);
  void release(const unsigned int& num_instances);
  unsigned int getIndex() const;
};


#endif //TEST_RESOURCE_H
