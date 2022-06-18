#pragma once

#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

namespace rocksdb {

class ZoneMutex {
 public:
  ZoneMutex();
  ~ZoneMutex();

  void Rdlock();
  void Wrlock();
  void Unlock();
  void AssertHeld() { }

 private:
  pthread_rwlock_t rwlock;
};
}