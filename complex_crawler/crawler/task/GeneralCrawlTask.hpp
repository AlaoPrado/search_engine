#pragma once

#include "../../../threadpool/CounterFlag.hpp"
#include "../../../threadpool/Task.hpp"
#include "../SiteAttributes.hpp"
#include "../action/Crawl.hpp"
#include "../scheduler/sync/SynchonizedPageGroupScheduler.hpp"
#include <CkSpider.h>
#include <cstddef>
#include <map>
#include <pthread.h>
#include <string>
#include <vector>

namespace search_engine {

class GeneralCrawlTask : public Task {
private:
  std::string storageDirectory;
  std::vector<std::string> *mustMatchPatterns, *avoidPatterns;
  std::size_t numPagesToCrawl, *allNumPagesToCrawl;
  SynchonizedPageGroupScheduler *pageScheduler;
  std::map<std::string, bool> *viewedUrls;
  std::map<std::string, SiteAttributes> *siteAttributesMap;
  std::map<std::string, Crawl::timePoint> *lastCrawlEndTimeMap;
  pthread_mutex_t *popMutex, *storeMutex, *pushMutex, *memoryMutex, *failMutex;
  CounterFlag *counterFlag;
  int threadId;

public:
  GeneralCrawlTask();
  ~GeneralCrawlTask();
  void set(std::string storageDirectory,
           std::vector<std::string> *mustMatchPatterns,
           std::vector<std::string> *avoidPatterns, std::size_t numPagesToCrawl,
           std::size_t *allNumPagesToCrawl,
           SynchonizedPageGroupScheduler *pageScheduler,
           std::map<std::string, bool> *viewedUrls,
           std::map<std::string, SiteAttributes> *siteAttributesMap,
           std::map<std::string, Crawl::timePoint> *lastCrawlEndTimeMap,
           pthread_mutex_t *popMutex, pthread_mutex_t *storeMutex,
           pthread_mutex_t *pushMutex, pthread_mutex_t *memoryMutex,
           pthread_mutex_t *failMutex, CounterFlag *counterFlag, int threadId);
  void run() override;
};

} // namespace search_engine
