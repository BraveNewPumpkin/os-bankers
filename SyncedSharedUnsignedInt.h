//
// Created by Kyle Bolton on 10/16/16.
//

#ifndef OS_BANKERS_SYNCEDSHAREDUNSIGNEDINT_H
#define OS_BANKERS_SYNCEDSHAREDUNSIGNEDINT_H

#include <string>
#include <memory>

#include <boost/interprocess/sync/interprocess_mutex.hpp>

using namespace boost::interprocess;

class SyncedSharedUnsignedInt {
private:
  interprocess_mutex mtx;
  const std::string name;
public:
  SyncedSharedUnsignedInt(const std::string& name, const unsigned int& initial) : name(name), num(initial) {};
  void lockNum();
  void unlockNum();
  std::unique_ptr<std::string> getName();
  unsigned int num;
};


#endif //OS_BANKERS_SYNCEDSHAREDUNSIGNEDINT_H
