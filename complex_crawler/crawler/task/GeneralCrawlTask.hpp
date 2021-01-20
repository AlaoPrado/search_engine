#pragma once

#include "../../../threadpool/Task.hpp"
#include "../SiteAttributes.hpp"
#include "../action/Crawl.hpp"
#include "../scheduler/PageScheduler.hpp"
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
  std::size_t numPagesToCrawl;
  PageScheduler *pageScheduler;
  std::map<std::string, bool> *viewedUrls;
  std::map<std::string, SiteAttributes> *siteAttributesMap;
  std::map<std::string, Crawl::timePoint> *lastCrawlEndTimeMap;
  pthread_mutex_t *popMutex, *storeMutex, *pushMutex;

public:
  GeneralCrawlTask(std::string storageDirectory,
                   std::vector<std::string> *mustMatchPatterns,
                   std::vector<std::string> *avoidPatterns,
                   std::size_t numPagesToCrawl, PageScheduler *pageScheduler,
                   std::map<std::string, bool> *viewedUrls,
                   std::map<std::string, SiteAttributes> *siteAttributesMap,
                   std::map<std::string, Crawl::timePoint> *lastCrawlEndTimeMap,
                   pthread_mutex_t *popMutex, pthread_mutex_t *storeMutex,
                   pthread_mutex_t *pushMutex);
  ~GeneralCrawlTask();
  void run() override;
};

} // namespace search_engine
