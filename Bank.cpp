//
// Created by Kyle Bolton on 10/6/16.
//

#include "Bank.h"

const int Bank::addResource(const int& num_instances) {
  resources.push_back(make_unique<Resource>(num_instances));
  return resources.size()-1;
}

unique_ptr<Resource>& Bank::getResource(const int& resource_id) {
  return resources[resource_id];
}
