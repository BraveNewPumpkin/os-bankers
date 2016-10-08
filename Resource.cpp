//
// Created by Kyle Bolton on 10/6/16.
//

#include "Resource.h"

Resource::Resource(const int num_instances) {
  available = num_instances;
  used = 0;
}
