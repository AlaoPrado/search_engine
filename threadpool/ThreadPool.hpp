/* Copyright 2021 Alan Prado Araújo and Igor Henrique Firmo Santos

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
Authors: alanpradoaraujo@gmail.com (Alan Prado Araújo) and
         igorfirmo5@gmail.com (Igor Henrique Firmo Santos) */

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
