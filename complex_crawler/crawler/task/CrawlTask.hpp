#pragma once

#include "../../../threadpool/Task.hpp"
#include "../../../utils/SynchronizedQueue.hpp"
#include "../Page.hpp"
#include "../SiteAttributes.hpp"
#include "../action/Crawl.hpp"
#include "CrawlTaskResult.hpp"
#include <CkSpider.h>
#include <pthread.h>
#include <string>
#include <vector>

namespace search_engine {

class CrawlTask : public Task {
private:
  pthread_mutex_t *memoryMutex;
  utils::SynchronizedQueue<CrawlTaskResult> *queue;
  CkSpider *spider;
  Page page;
  std::vector<std::string> *mustMatchPatterns;
  std::vector<std::string> *avoidPatterns;
  SiteAttributes *siteAttribute;
  Crawl::timePoint *lastCrawlEndTime;
  bool useLastCrawlEndTime;

public:
  CrawlTask(pthread_mutex_t *memoryMutex,
            utils::SynchronizedQueue<CrawlTaskResult> *queue, CkSpider *spider,
            Page page, std::vector<std::string> *mustMatchPatterns,
            std::vector<std::string> *avoidPatterns,
            SiteAttributes *siteAttribute, Crawl::timePoint *lastCrawlEndTime,
            bool useLastCrawlEndTime);
  ~CrawlTask();
  void run() override;
};

} // namespace search_engine
