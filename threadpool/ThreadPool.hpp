#pragma once

#include "SynchronizedQueue.hpp"
#include <pthread.h>

class ThreadPool {
private:
  pthread_t *pool;
  SynchronizedQueue *workQueue;

  static void *threadRun(void *);

public:
  ThreadPool(int size);
  ~ThreadPool();
  void addTask(Task *task);
  int getSize();
  int size;
};