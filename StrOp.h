//
// Created by Kyle Bolton on 10/18/16.
//

#ifndef OS_BANKERS_STROP_H
#define OS_BANKERS_STROP_H

#include <vector>
#include <string>
#include <sstream>

namespace StrOp {
  void split(const char& delim, const std::string& string_to_split, std::vector<std::string>& elements);

  std::vector<std::string> split(const char& delim, const std::string& string_to_split);

  template<typename T>
  std::string join(const std::string& delim, std::vector<T>& elements);
};


#endif //OS_BANKERS_STROP_H
