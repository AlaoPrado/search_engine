#pragma once

#include "../../../threadpool/CounterFlag.hpp"
#include "../../../threadpool/Task.hpp"
#include "../scheduler/PageScheduler.hpp"
#include "CrawlTaskResult.hpp"
#include <map>
#include <pthread.h>
#include <string>

namespace search_engine {

class SchedulerPushTask : public Task {
private:
  pthread_mutex_t *memoryMutex;
  PageScheduler *pageScheduler;
  CrawlTaskResult *crawlTaskResult;
  std::map<std::string, bool> *viewedUrls;
  std::size_t numPagesToCrawl;
  CounterFlag *processCounterFlag;

public:
  SchedulerPushTask(pthread_mutex_t *memoryMutex, PageScheduler *pageScheduler,
                    CrawlTaskResult *crawlTaskResult,
                    std::map<std::string, bool> *viewedUrls,
                    std::size_t numPagesToCrawl,
                    CounterFlag *processCounterFlag);
  ~SchedulerPushTask();
  void run() override;
};

} // namespace search_engine
