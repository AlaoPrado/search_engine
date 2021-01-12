#pragma once

#include "../../../threadpool/Task.hpp"
#include "../../../threadpool/ThreadPool.hpp"
#include "../action/Crawl.hpp"
#include "../scheduler/sync/SynchonizedPageGroupScheduler.hpp"
#include <map>
#include <string>

namespace search_engine {

class SchedulerPopAllTask : public Task {
private:
  std::size_t numExpectedPops;
  pthread_mutex_t *memoryMutex;
  SynchonizedPageGroupScheduler *pageGroupScheduler;
  ThreadPool *crawlPool;
  utils::SynchronizedQueue<CkSpider> *queue;
  std::vector<std::string> *mustMatchPatterns, *avoidPatterns;
  std::map<std::string, double> *totalTimeMap;
  std::map<std::string, Crawl::timePoint> *lastCrawlEndTimeMap;

public:
  SchedulerPopAllTask(
      std::size_t numExpectedPops, pthread_mutex_t *memoryMutex,
      SynchonizedPageGroupScheduler *pageGroupScheduler, ThreadPool *crawlPool,
      utils::SynchronizedQueue<CkSpider> *queue,
      std::vector<std::string> *mustMatchPatterns,
      std::vector<std::string> *avoidPatterns,
      std::map<std::string, double> *totalTimeMap,
      std::map<std::string, Crawl::timePoint> *lastCrawlEndTimeMap);
  ~SchedulerPopAllTask();
  void run() override;
};

} // namespace search_engine
