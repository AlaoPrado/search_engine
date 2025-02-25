/* Copyright 2021 Alan Prado Araújo

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
Author: alanpradoaraujo@gmail.com (Alan Prado Araújo) */
#include "SynchonizedPageGroupScheduler.hpp"
#include "../PageGroupScheduler.hpp"
#include "../PageScheduler.hpp"
#include <iostream>
#include <pthread.h>
#include <string>

namespace search_engine {

SynchonizedPageGroupScheduler::SynchonizedPageGroupScheduler(
    pthread_mutex_t *memoryMutex) {
  this->pageGroupScheduler = new PageGroupScheduler();
  this->memoryMutex = memoryMutex;
  pthread_mutex_init(&mutex, 0);
  pthread_cond_init(&cond, 0);
}

SynchonizedPageGroupScheduler::~SynchonizedPageGroupScheduler() {
  delete this->pageGroupScheduler;
  pthread_mutex_destroy(&mutex);
  pthread_cond_destroy(&cond);
}

Page SynchonizedPageGroupScheduler::pop() {
  pthread_mutex_lock(&mutex);

  while (this->pageGroupScheduler->blocked()) {
    pthread_cond_wait(&cond, &mutex);
  }

  if (this->memoryMutex != NULL) {
    pthread_mutex_lock(this->memoryMutex);
    // std::cout << "SynchonizedPageGroupScheduler lock memory pop" <<
    // std::endl;
  }

  Page page = this->pageGroupScheduler->pop();

  if (this->memoryMutex != NULL) {
    // std::cout << "SynchonizedPageGroupScheduler unlock memory pop" <<
    // std::endl;
    pthread_mutex_unlock(this->memoryMutex);
  }

  pthread_mutex_unlock(&mutex);
  return page;
}

void SynchonizedPageGroupScheduler::push(Page page) {
  pthread_mutex_lock(&mutex);

  if (this->memoryMutex != NULL) {
    pthread_mutex_lock(this->memoryMutex);

    // std::cout << "SynchonizedPageGroupScheduler lock memory push" <<
    // std::endl;
  }

  this->pageGroupScheduler->push(page);

  if (this->memoryMutex != NULL) {
    // std::cout << "SynchonizedPageGroupScheduler unlock memory push"
    //           << std::endl;
    pthread_mutex_unlock(this->memoryMutex);
  }

  pthread_cond_signal(&cond);
  pthread_mutex_unlock(&mutex);
}

std::size_t SynchonizedPageGroupScheduler::size() {
  pthread_mutex_lock(&mutex);
  std::size_t numPages = this->pageGroupScheduler->size();
  pthread_mutex_unlock(&mutex);
  return numPages;
}

bool SynchonizedPageGroupScheduler::empty() {
  pthread_mutex_lock(&mutex);
  bool codition = this->pageGroupScheduler->empty();
  pthread_mutex_unlock(&mutex);
  return codition;
}

void SynchonizedPageGroupScheduler::finishWork(std::string url) {
  pthread_mutex_lock(&mutex);

  if (this->memoryMutex != NULL) {
    pthread_mutex_lock(this->memoryMutex);
  }

  this->pageGroupScheduler->finishWork(url);

  if (this->memoryMutex != NULL) {
    pthread_mutex_unlock(this->memoryMutex);
  }
  
  pthread_cond_signal(&cond);
  pthread_mutex_unlock(&mutex);
}

bool SynchonizedPageGroupScheduler::blocked() {
  bool condition;
  pthread_mutex_lock(&mutex);
  condition = this->pageGroupScheduler->blocked();
  pthread_mutex_unlock(&mutex);
  return condition;
}

} // namespace search_engine
