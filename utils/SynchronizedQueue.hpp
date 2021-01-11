#pragma once

#include <pthread.h>
#include <queue>

namespace search_engine {
namespace utils {

template <typename T> class SynchronizedQueue {
private:
  std::queue<T *> *queue;
  pthread_mutex_t mutex;
  pthread_cond_t cond;

public:
  SynchronizedQueue();
  ~SynchronizedQueue();
  void push(T *obj);
  T *pop();
};

template <typename T> SynchronizedQueue<T>::SynchronizedQueue() {
  this->queue = new std::queue<T *>();
  pthread_mutex_init(&mutex, 0);
  pthread_cond_init(&cond, 0);
}

template <typename T> SynchronizedQueue<T>::~SynchronizedQueue() {
  while (!this->queue->empty()) {
    delete this->queue->front();
    this->queue->pop();
  }
  delete this->queue;

  pthread_mutex_destroy(&mutex);
  pthread_cond_destroy(&cond);
}

template <typename T> void SynchronizedQueue<T>::push(T *obj) {
  pthread_mutex_lock(&mutex);
  this->queue->push(obj);
  pthread_cond_signal(&cond);
  pthread_mutex_unlock(&mutex);
}

template <typename T> T *SynchronizedQueue<T>::pop() {
  T *obj = NULL;
  pthread_mutex_lock(&mutex);

  while (this->queue->empty())
    pthread_cond_wait(&cond, &mutex);

  obj = this->queue->front();
  this->queue->pop();

  pthread_mutex_unlock(&mutex);
  return obj;
}

} // namespace utils
} // namespace search_engine
