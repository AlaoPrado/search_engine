#pragma once

#include <pthread.h>

namespace search_engine {

class MasterFlag {
private:
  pthread_mutex_t mutex;
  pthread_cond_t cond;
  int tasksDone, taskLimit;

public:
  MasterFlag(int taskLimit);
  ~MasterFlag();
  void reset(int taskLimit);
  void increaseTaskNum(int);
  void wait();
  void signal();
};

} // namespace search_engine
