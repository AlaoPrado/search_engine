#include "ThreadPool.hpp"
#include "../utils/SynchronizedQueue.hpp"
#include <stdexcept>

namespace search_engine {

void *ThreadPool::threadRun(void *param) {
  Task *task;
  utils::SynchronizedQueue<Task> *workQueue;
  workQueue = reinterpret_cast<utils::SynchronizedQueue<Task> *>(param);
  task = workQueue->pop();
  while (task != NULL) {
    task->run();
    task = workQueue->pop();
  }
  return NULL;
}

ThreadPool::ThreadPool(int size) {
  this->size = size;
  this->pool = new pthread_t[size];
  this->workQueue = new utils::SynchronizedQueue<Task>();
  for (int i = 0; i < this->size; i++) {
    if (!pthread_create(&(this->pool[i]), NULL, this->threadRun,
                        (void *)this->workQueue)) {
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
}

void ThreadPool::addTask(Task *task) { workQueue->push(task); }

int ThreadPool::getSize() { return this->size; }

} // namespace search_engine
