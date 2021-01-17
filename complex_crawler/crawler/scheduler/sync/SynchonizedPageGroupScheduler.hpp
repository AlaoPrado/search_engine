#pragma once

#include "../PageGroupScheduler.hpp"
#include "../PageScheduler.hpp"
#include <pthread.h>
#include <string>

namespace search_engine {

class SynchonizedPageGroupScheduler : public PageGroupScheduler {
private:
  PageGroupScheduler *pageGroupScheduler;
  pthread_mutex_t mutex;
  pthread_cond_t cond;
  pthread_mutex_t *memoryMutex;
  std::size_t numPops, numExpectedPops;

public:
  SynchonizedPageGroupScheduler(std::size_t numExpectedPops,
                                pthread_mutex_t *memoryMutex = NULL);
  ~SynchonizedPageGroupScheduler();
  virtual Page pop() override;
  virtual void push(Page url) override;
  virtual std::size_t size() override;
  virtual bool empty() override;
  virtual void finishWork(std::string url) override;
  virtual bool blocked() override;
};

} // namespace search_engine
