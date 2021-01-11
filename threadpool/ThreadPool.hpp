#pragma once

#include "../utils/SynchronizedQueue.hpp"
#include "Task.hpp"
#include <pthread.h>

namespace search_engine {
  
class ThreadPool {
private:
  pthread_t *pool;
  utils::SynchronizedQueue<Task> *workQueue;

  static void *threadRun(void *);

public:
  ThreadPool(int size);
  ~ThreadPool();
  void addTask(Task *task);
  int getSize();
  int size;
};

} // namespace search_engine
