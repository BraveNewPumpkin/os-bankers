//
// Created by Kyle Bolton on 10/18/16.
//

#ifndef OS_BANKERS_STRINGOP_H
#define OS_BANKERS_STRINGOP_H

#include <vector>

namepace StringOp {

   void split(const std::string &s, char delim, std::vector<std::string> &elems) {
     std::stringstream ss;
     ss.str(s);
     std::string item;
     while (std::getline(ss, item, delim)) {
       elems.push_back(item);
     }
   }

   std::vector<std::string> split(const std::string &s, char delim) {
     std::vector<std::string> elems;
     split(s, delim, elems);
     return elems;
   }

   std::string join(const std::string& delim, std::vector<std::string>& elements){
     std::ostringstream stream;
     std::copy(requested_resources.begin(), requested_resources.end(), std::ostream_iterator<unsigned int>(stream, delim));
     stream << requested_resources.back();
     std::string resources_string = stream.str();
     resources_string.pop_back();
     resources_string.pop_back();
     return resources_string;
   }

   template<typename T>
   std::string join(const std::string& delim, std::vector<T>& elements){
   }
}

#endif //OS_BANKERS_STRINGOP_H
