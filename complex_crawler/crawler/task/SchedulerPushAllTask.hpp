#pragma once

#include "../../../threadpool/CounterFlag.hpp"
#include "../../../threadpool/Task.hpp"
#include "../../../utils/SynchronizedQueue.hpp"
#include "../scheduler/sync/SynchonizedPageGroupScheduler.hpp"
#include <CkSpider.h>
#include <map>
#include <pthread.h>
#include <string>

namespace search_engine {

class SchedulerPushAllTask : public Task {
private:
  CounterFlag *counterFlag;
  std::size_t numPagesToCrawl;
  pthread_mutex_t *memoryMutex;
  utils::SynchronizedQueue<CkSpider> *spiderQueue;
  SynchonizedPageGroupScheduler *pageGroupScheduler;
  std::map<std::string, bool> *viewedUrls;
  std::string storageDirectory;
  bool verbose;

public:
  SchedulerPushAllTask(CounterFlag *counterFlag, std::size_t numPagesToCrawl,
                       pthread_mutex_t *memoryMutex,
                       utils::SynchronizedQueue<CkSpider> *spiderQueue,
                       SynchonizedPageGroupScheduler *pageGroupScheduler,
                       std::map<std::string, bool> *viewedUrls,
                       std::string storageDirectory, bool verbose);
  ~SchedulerPushAllTask();
  void run() override;
};

} // namespace search_engine
