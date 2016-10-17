//
// Created by Kyle Bolton on 10/16/16.
//

#include "SyncedSharedUnsignedInt.h"

void SyncedSharedUnsignedInt::lockNum() {
  mtx.lock();
}

void SyncedSharedUnsignedInt::unlockNum() {
  mtx.unlock();
}

std::unique_ptr<std::string> SyncedSharedUnsignedInt::getName() {
  return std::unique_ptr<std::string>(new std::string(name));
}
