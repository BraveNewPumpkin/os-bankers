//
// Created by Kyle Bolton on 10/6/16.
//

#include "Bank.h"

Bank::Bank() {
}

const int Bank::addResource(const int num_instances) {
  //TODO vector::emplace? create resource (dynamically?) and put in vector
}

void Bank::addClaim(const int pid, const int resource_id) {
  //TODO implement
}

resource & Bank ::getResource(const int resource_id) {
  return resources[resource_id];
}
