//
// Created by Kyle Bolton on 10/6/16.
//

#include "Resource.h"

Resource::operator std::string() const {
  return "index: " + std::to_string(index) + " available: " + std::to_string(available) + " used: " + std::to_string(used);
}

unsigned int Resource::getAvailable() const {
  return Resource::available;
}

unsigned int Resource::getUsed() const {
  return Resource::used;
}

void Resource::allocate(const unsigned int &num_instances) {
  if(available<num_instances) {
    throw std::runtime_error("cannot allocate " + std::to_string(num_instances) + " instances of resource with only "
      + std::to_string(available) + " instances available");
  }
  available -= num_instances;
  used += num_instances;
}

void Resource::release(const unsigned int &num_instances) {
  if(used<num_instances) {
    throw std::runtime_error("cannot release " + std::to_string(num_instances) + " instances of resource with only "
      + std::to_string(available) + " instances used");
  }
  available += num_instances;
  used -= num_instances;
}

unsigned int Resource::getIndex() const {
  return index;
}
