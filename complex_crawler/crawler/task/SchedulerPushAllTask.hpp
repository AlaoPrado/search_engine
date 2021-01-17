#pragma once

#include "../../../threadpool/CounterFlag.hpp"
#include "../../../threadpool/Task.hpp"
#include "../../../threadpool/ThreadPool.hpp"
#include "../../../utils/SynchronizedQueue.hpp"
#include "../scheduler/sync/SynchonizedPageGroupScheduler.hpp"
#include "CrawlTaskResult.hpp"
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
  utils::SynchronizedQueue<CrawlTaskResult> *crawlTaskResultQueue;
  SynchonizedPageGroupScheduler *pageGroupScheduler;
  std::map<std::string, bool> *viewedUrls;
  std::string storageDirectory;
  ThreadPool *schedulerPopPool, *storePool;
  bool verbose;

public:
  SchedulerPushAllTask(
      CounterFlag *counterFlag, std::size_t numPagesToCrawl,
      pthread_mutex_t *memoryMutex,
      utils::SynchronizedQueue<CrawlTaskResult> *crawlTaskResultQueue,
      SynchonizedPageGroupScheduler *pageGroupScheduler,
      std::map<std::string, bool> *viewedUrls, std::string storageDirectory,
      ThreadPool *schedulerPopPool, ThreadPool *storePool, bool verbose);
  ~SchedulerPushAllTask();
  void run() override;
};

} // namespace search_engine
