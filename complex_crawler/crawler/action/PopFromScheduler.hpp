#pragma once

#include "../Page.hpp"
#include "../SiteAttributes.hpp"
#include "../scheduler/PageScheduler.hpp"
#include "Crawl.hpp"
#include <chrono>
#include <map>
#include <pthread.h>
#include <string>

namespace search_engine {
class PopFromScheduler {
private:
  PopFromScheduler();

public:
  static void pop(PageScheduler &pageScheduler, Page &page,
                  std::string &baseUrl, bool &useLastCrawlEndTime,
                  std::map<std::string, SiteAttributes> &siteAttributesMap,
                  std::map<std::string, Crawl::timePoint> &lastCrawlEndTimeMap,
                  pthread_mutex_t *memoryMutex = NULL,
                  pthread_mutex_t *popMutex = NULL);
};

} // namespace search_engine
