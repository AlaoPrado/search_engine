#pragma once

#include "../../../threadpool/Task.hpp"
#include "../../../threadpool/ThreadPool.hpp"
#include "../../../utils/SynchronizedQueue.hpp"
#include "../action/Crawl.hpp"
#include "../scheduler/sync/SynchonizedPageGroupScheduler.hpp"
#include <map>
#include <pthread.h>
#include <string>

namespace search_engine {

class SchedulerPopAllTask : public Task {
private:
  std::size_t numExpectedPops;
  pthread_mutex_t *memoryMutex;
  SynchonizedPageGroupScheduler *pageGroupScheduler;
  ThreadPool *crawlPool;
  utils::SynchronizedQueue<CkSpider> *spiderQueue;
  std::vector<std::string> *mustMatchPatterns, *avoidPatterns;
  std::map<std::string, double> *totalTimeMap;
  std::map<std::string, Crawl::timePoint> *lastCrawlEndTimeMap;
  pthread_mutex_t *crawlMutex;

public:
  SchedulerPopAllTask(
      std::size_t numExpectedPops, pthread_mutex_t *memoryMutex,
      SynchonizedPageGroupScheduler *pageGroupScheduler, ThreadPool *crawlPool,
      utils::SynchronizedQueue<CkSpider> *spiderQueue,
      std::vector<std::string> *mustMatchPatterns,
      std::vector<std::string> *avoidPatterns,
      std::map<std::string, double> *totalTimeMap,
      std::map<std::string, Crawl::timePoint> *lastCrawlEndTimeMap,
      pthread_mutex_t *crawlMutex);
  ~SchedulerPopAllTask();
  void run() override;
};

} // namespace search_engine
