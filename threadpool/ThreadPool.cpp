#include "ThreadPool.hpp"
#include "../utils/SynchronizedQueue.hpp"
#include <pthread.h>
#include <stdexcept>
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
    if (!pthread_create(&(this->pool[i]), NULL, this->threadRun,
                        (void *)this->threadParam)) {
      throw std::logic_error("Error: failed to create thread " + i);
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
