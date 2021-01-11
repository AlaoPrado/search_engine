#pragma once

#include "TaskQueue.hpp"
#include <pthread.h>

class ThreadPool {
private:
  pthread_t *pool;
  TaskQueue *workQueue;

  static void *threadRun(void *);

public:
  ThreadPool(int size);
  ~ThreadPool();
  void addTask(Task *task);
  int getSize();
  int size;
};
