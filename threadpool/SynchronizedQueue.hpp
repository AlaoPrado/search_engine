#pragma once

#include "task/Task.hpp"
#include <pthread.h>
#include <queue>

class SynchronizedQueue {
private:
  std::queue<Task *> *taskQueue;
  pthread_mutex_t mutex;
  pthread_cond_t cond;

public:
  SynchronizedQueue();
  ~SynchronizedQueue();
  void push(Task *task);
  Task *pop();
};
