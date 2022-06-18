#include "zone_mutex.h"
namespace rocksdb {

static void PthreadCall(const char* label, int result) {
  if (result != 0) {
    fprintf(stderr, "pthread %s: %s\n", label, strerror(result));
    abort();
  }
}

ZoneMutex::ZoneMutex() { PthreadCall("init mutex", pthread_rwlock_init(&rwlock, NULL)); }

ZoneMutex::~ZoneMutex() { PthreadCall("destroy mutex", pthread_rwlock_destroy(&rwlock)); }

void ZoneMutex::Rdlock() { PthreadCall("rdlock", pthread_rwlock_rdlock(&rwlock)); }

void ZoneMutex::Wrlock() { PthreadCall("wrlock", pthread_rwlock_wrlock(&rwlock)); }

void ZoneMutex::Unlock() { PthreadCall("unlock", pthread_rwlock_unlock(&rwlock)); }

}