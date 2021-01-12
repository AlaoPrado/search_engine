#include "SynchonizedPageGroupScheduler.hpp"
#include "../PageGroupScheduler.hpp"
#include "../PageScheduler.hpp"
#include <iostream>
#include <pthread.h>
#include <string>

namespace search_engine {

SynchonizedPageGroupScheduler::SynchonizedPageGroupScheduler(
    std::size_t numExpectedPops, pthread_mutex_t *memoryMutex) {
  this->pageGroupScheduler = new PageGroupScheduler();
  this->numExpectedPops = numExpectedPops;
  this->memoryMutex = memoryMutex;
  this->numPops = 0;
  pthread_mutex_init(&mutex, 0);
  pthread_cond_init(&cond, 0);
}

SynchonizedPageGroupScheduler::~SynchonizedPageGroupScheduler() {
  delete this->pageGroupScheduler;
  pthread_mutex_destroy(&mutex);
  pthread_cond_destroy(&cond);
}

std::string SynchonizedPageGroupScheduler::pop() {
  pthread_mutex_lock(&mutex);

  std::string url("");
  while (this->numPops < this->numExpectedPops &&
         this->pageGroupScheduler->empty() && url.length() == 0) {
    pthread_cond_wait(&cond, &mutex);

    if (this->memoryMutex != NULL) {
      pthread_mutex_lock(this->memoryMutex);
      std::cout << "SynchonizedPageGroupScheduler lock memory pop" << std::endl;
    }

    url = this->pageGroupScheduler->pop();

    if (this->memoryMutex != NULL) {
      std::cout << "SynchonizedPageGroupScheduler unlock memory pop"
                << std::endl;
      pthread_mutex_unlock(this->memoryMutex);
    }
  }

  if (url.length() == 0) {
    if (this->memoryMutex != NULL) {
      pthread_mutex_lock(this->memoryMutex);
      std::cout << "SynchonizedPageGroupScheduler lock memory pop" << std::endl;
    }

    url = this->pageGroupScheduler->pop();

    if (this->memoryMutex != NULL) {
      std::cout << "SynchonizedPageGroupScheduler unlock memory pop"
                << std::endl;
      pthread_mutex_unlock(this->memoryMutex);
    }
  }

  this->numPops++;

  pthread_mutex_unlock(&mutex);
  return url;
}

void SynchonizedPageGroupScheduler::push(std::string url) {
  pthread_mutex_lock(&mutex);

  if (this->memoryMutex != NULL) {
    pthread_mutex_lock(this->memoryMutex);
    std::cout << "SynchonizedPageGroupScheduler lock memory push" << std::endl;
    this->pageGroupScheduler->push(url);
    std::cout << "SynchonizedPageGroupScheduler unlock memory push"
              << std::endl;
    pthread_mutex_unlock(this->memoryMutex);
  } else {
    this->pageGroupScheduler->push(url);
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
  this->pageGroupScheduler->finishWork(url);
  pthread_cond_signal(&cond);
  pthread_mutex_unlock(&mutex);
}

} // namespace search_engine
