//
// Created by Kyle Bolton on 10/6/16.
//

#include "Resource.h"

Resource::Resource(const int& num_instances) {
  available = num_instances;
  used = 0;
}

unsigned int Resource::getAvailable() const {
  return Resource::available;
}

unsigned int Resource::getUsed() const {
  return Resource::used;
}

void Resource::allocate(const unsigned int &num_instances) {
  if(available<num_instances) {
    throw runtime_error("cannot allocate " + to_string(num_instances) + " instances of resource with only " + to_string(available) + " instances available");
  }
  available -= num_instances;
  used += num_instances;
}

void Resource::release(const unsigned int &num_instances) {
  if(used<num_instances) {
    throw runtime_error("cannot release " + to_string(num_instances) + " instances of resource with only " + to_string(available) + " instances used");
  }
  available += num_instances;
  used -= num_instances;
}
