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

#include "ThreadPool.hpp"
#include "../utils/SynchronizedQueue.hpp"
#include <pthread.h>
#include <stdexcept>
#include <string>
#include <utility>

namespace search_engine {

ThreadParam::ThreadParam(utils::SynchronizedQueue<Task> *workQueue,
                         pthread_mutex_t *memoryMutex)
    : workQueue(workQueue), memoryMutex(memoryMutex) {}

utils::SynchronizedQueue<Task> *ThreadParam::getWorkQueue() {
  return this->workQueue;
}

pthread_mutex_t *ThreadParam::getMemoryMutex() { return this->memoryMutex; }

void *ThreadPool::threadRun(void *param) {
  Task *task;
  ThreadParam *threadParam = reinterpret_cast<ThreadParam *>(param);
  utils::SynchronizedQueue<Task> *workQueue = threadParam->getWorkQueue();
  pthread_mutex_t *memoryMutex = threadParam->getMemoryMutex();
  task = workQueue->pop();
  while (task != NULL) {
    task->run();

    if (memoryMutex != NULL) {
      pthread_mutex_lock(memoryMutex);
      delete task;
      pthread_mutex_unlock(memoryMutex);
    }

    task = workQueue->pop();
  }
  return NULL;
}

ThreadPool::ThreadPool(int size, pthread_mutex_t *memoryMutex) {
  this->size = size;
  this->memoryMutex = memoryMutex;
  this->pool = new pthread_t[size];
  this->workQueue = new utils::SynchronizedQueue<Task>(this->memoryMutex);
  this->threadParam = new ThreadParam(this->workQueue, this->memoryMutex);
  for (int i = 0; i < this->size; i++) {
    int error = pthread_create(&(this->pool[i]), NULL, this->threadRun,
                               (void *)this->threadParam);
    if (error != 0) {
      throw std::logic_error("Error: failed to create thread " +
                             std::to_string(i) + " with code " +
                             std::to_string(error));
    }
  }
}

ThreadPool::~ThreadPool() {
  for (int i = 0; i < this->size; i++)
    this->workQueue->push(NULL);
  for (int i = 0; i < this->size; i++)
    pthread_join(this->pool[i], NULL);
  delete this->workQueue;
  delete this->pool;
  delete this->threadParam;
}

void ThreadPool::addTask(Task *task) { workQueue->push(task); }

int ThreadPool::getSize() { return this->size; }

} // namespace search_engine
