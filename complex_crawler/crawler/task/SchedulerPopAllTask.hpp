#pragma once

#include "../../../threadpool/Task.hpp"
#include "../../../threadpool/ThreadPool.hpp"
#include "../../../utils/SynchronizedQueue.hpp"
#include "../action/Crawl.hpp"
#include "../scheduler/sync/SynchonizedPageGroupScheduler.hpp"
#include "CrawlTaskResult.hpp"
#include <map>
#include <pthread.h>
#include <string>

namespace search_engine {

class SchedulerPopAllTask : public Task {
private:
  pthread_mutex_t *memoryMutex;
  std::size_t numExpectedPops;
  SynchonizedPageGroupScheduler *pageGroupScheduler;
  ThreadPool *crawlPool;
  utils::SynchronizedQueue<CrawlTaskResult> *crawlTaskResultQueue;
  std::vector<std::string> *mustMatchPatterns, *avoidPatterns;
  std::map<std::string, SiteAttributes> *siteAttributesMap;
  std::map<std::string, Crawl::timePoint> *lastCrawlEndTimeMap;

public:
  SchedulerPopAllTask(
      pthread_mutex_t *memoryMutex, std::size_t numExpectedPops,
      SynchonizedPageGroupScheduler *pageGroupScheduler, ThreadPool *crawlPool,
      utils::SynchronizedQueue<CrawlTaskResult> *crawlTaskResultQueue,
      std::vector<std::string> *mustMatchPatterns,
      std::vector<std::string> *avoidPatterns,
      std::map<std::string, SiteAttributes> *siteAttributesMap,
      std::map<std::string, Crawl::timePoint> *lastCrawlEndTimeMap);
  ~SchedulerPopAllTask();
  void run() override;
};

} // namespace search_engine
