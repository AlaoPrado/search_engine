#include "PopFromScheduler.hpp"
#include "../../../utils/Url.hpp"
#include "../Page.hpp"
#include "../SiteAttributes.hpp"
#include "Crawl.hpp"
#include <chrono>
#include <iostream>
#include <map>
#include <pthread.h>
#include <string>

namespace search_engine {

void PopFromScheduler::pop(
    PageScheduler &pageScheduler, Page &page, std::string &baseUrl,
    bool &useLastCrawlEndTime,
    std::map<std::string, SiteAttributes> &siteAttributesMap,
    std::map<std::string, Crawl::timePoint> &lastCrawlEndTimeMap,
    pthread_mutex_t *memoryMutex, pthread_mutex_t *popMutex) {
  page = pageScheduler.pop();
  baseUrl = utils::baseUrl(page.getUrl());

  if (popMutex != NULL) {
    pthread_mutex_lock(popMutex);
  }

  auto it = lastCrawlEndTimeMap.find(baseUrl);
  if (it == lastCrawlEndTimeMap.end()) {
    if (memoryMutex != NULL) {
      pthread_mutex_lock(memoryMutex);
    }

    siteAttributesMap.operator[](baseUrl) = SiteAttributes();
    lastCrawlEndTimeMap.operator[](baseUrl) = std::chrono::steady_clock::now();

    if (memoryMutex != NULL) {
      pthread_mutex_unlock(memoryMutex);
    }

    useLastCrawlEndTime = false;
  } else {
    useLastCrawlEndTime = true;
  }

  if (popMutex != NULL) {
    pthread_mutex_unlock(popMutex);
  }
}

} // namespace search_engine
