#pragma once

#include "../utils/SynchronizedQueue.hpp"
#include "Task.hpp"
#include <pthread.h>

namespace search_engine {

class ThreadParam {
private:
  utils::SynchronizedQueue<Task> *workQueue;
  pthread_mutex_t *memoryMutex;

public:
  ThreadParam(utils::SynchronizedQueue<Task> *workQueue,
              pthread_mutex_t *memoryMutex);
  utils::SynchronizedQueue<Task> *getWorkQueue();
  pthread_mutex_t *getMemoryMutex();
};

class ThreadPool {
private:
  pthread_t *pool;
  utils::SynchronizedQueue<Task> *workQueue;
  pthread_mutex_t *memoryMutex;
  ThreadParam *threadParam;

  static void *threadRun(void *);

public:
  ThreadPool(int size, pthread_mutex_t *memoryMutex = NULL);
  ~ThreadPool();
  void addTask(Task *task);
  int getSize();
  int size;
};

} // namespace search_engine
