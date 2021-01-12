#pragma once

#include <pthread.h>

namespace search_engine {

class CounterFlag {
private:
  pthread_mutex_t mutex;
  pthread_cond_t cond;
  int tasksDone, taskLimit;

public:
  CounterFlag(int taskLimit);
  ~CounterFlag();
  void reset(int taskLimit);
  void increaseTaskNum(int);
  void wait();
  void signal();
};

} // namespace search_engine
