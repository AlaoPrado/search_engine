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

#include <pthread.h>
#include <queue>

namespace search_engine {
namespace utils {

template <typename T> class SynchronizedQueue {
private:
  std::queue<T *> *queue;
  pthread_mutex_t mutex;
  pthread_cond_t cond;
  pthread_mutex_t *memoryMutex;

public:
  SynchronizedQueue(pthread_mutex_t *memoryMutex = NULL);
  ~SynchronizedQueue();
  void push(T *obj);
  T *pop();
};

template <typename T>
SynchronizedQueue<T>::SynchronizedQueue(pthread_mutex_t *memoryMutex) {
  this->memoryMutex = memoryMutex;
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

  if (this->memoryMutex != NULL) {
    pthread_mutex_lock(this->memoryMutex);
    this->queue->push(obj);
    pthread_mutex_unlock(this->memoryMutex);
  } else {
    this->queue->push(obj);
  }

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
