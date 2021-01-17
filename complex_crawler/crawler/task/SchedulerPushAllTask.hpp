#pragma once

#include "../../../threadpool/CounterFlag.hpp"
#include "../../../threadpool/Task.hpp"
#include "../../../threadpool/ThreadPool.hpp"
#include "../../../utils/SynchronizedQueue.hpp"
#include "../SiteAttributes.hpp"
#include "../action/Crawl.hpp"
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
  SynchonizedPageGroupScheduler *pageGroupScheduler;
  ThreadPool *crawlPool;
  utils::SynchronizedQueue<CrawlTaskResult> *crawlTaskResultQueue;
  std::vector<std::string> *mustMatchPatterns;
  std::vector<std::string> *avoidPatterns;
  std::map<std::string, SiteAttributes> *siteAttributesMap;
  std::map<std::string, Crawl::timePoint> *lastCrawlEndTimeMap;
  ThreadPool *schedulerPopPool;
  std::map<std::string, bool> *viewedUrls;
  std::string storageDirectory;
  ThreadPool *storePool;
  bool verbose;

public:
  SchedulerPushAllTask(
      CounterFlag *counterFlag, std::size_t numPagesToCrawl,
      pthread_mutex_t *memoryMutex,
      SynchonizedPageGroupScheduler *pageGroupScheduler, ThreadPool *crawlPool,
      utils::SynchronizedQueue<CrawlTaskResult> *crawlTaskResultQueue,
      std::vector<std::string> *mustMatchPatterns,
      std::vector<std::string> *avoidPatterns,
      std::map<std::string, SiteAttributes> *siteAttributesMap,
      std::map<std::string, Crawl::timePoint> *lastCrawlEndTimeMap,
      ThreadPool *schedulerPopPool, std::map<std::string, bool> *viewedUrls,
      std::string storageDirectory, ThreadPool *storePool, bool verbose);
  ~SchedulerPushAllTask();
  void run() override;
};

} // namespace search_engine
