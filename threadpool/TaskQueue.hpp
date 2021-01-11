#pragma once

#include "Task.hpp"
#include <pthread.h>
#include <queue>

class TaskQueue {
private:
  std::queue<Task *> *taskQueue;
  pthread_mutex_t mutex;
  pthread_cond_t cond;

public:
  TaskQueue();
  ~TaskQueue();
  void push(Task *task);
  Task *pop();
};
