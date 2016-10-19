//
// Created by Kyle Bolton on 10/18/16.
//

#include "StrOp.h"

void StrOp::split(const char& delim, const std::string& string_to_split, std::vector<std::string>& elements) {
  std::stringstream ss;
  ss.str(string_to_split);
  std::string item;
  while (std::getline(ss, item, delim)) {
    elements.push_back(item);
  }
}

std::vector<std::string> StrOp::split(const char& delim, const std::string& string_to_split) {
  std::vector<std::string> elements;
  StrOp::split(delim, string_to_split, elements);
  return elements;
}

template<typename T>
std::string StrOp::join(const std::string& delim, std::vector<T>& elements){
  std::vector<std::string> stringified_elements;
  for(const T& element: elements){
    stringified_elements.push_back(std::to_string(element));
  }
  return join(delim, stringified_elements);
}

template<>
std::string StrOp::join<std::string>(const std::string& delim, std::vector<std::string>& elements){
  std::ostringstream stream;
  std::copy(elements.begin(), elements.end(), std::ostream_iterator<std::string>(stream, delim.c_str()));
  stream << elements.back();
  std::string joined_string = stream.str();
  joined_string.pop_back();
  joined_string.pop_back();
  return joined_string;
}

template<>
std::string StrOp::join<unsigned int>(const std::string& delim, std::vector<unsigned int>& elements){
  std::vector<std::string> stringified_elements;
  for(const unsigned int& element: elements){
    stringified_elements.push_back(std::to_string(element));
  }
  return join(delim, stringified_elements);
}
