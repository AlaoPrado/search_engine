#include "PopFromScheduler.hpp"
#include "../../../utils/Url.hpp"
#include "../SiteAttributes.hpp"
#include "Crawl.hpp"
#include <chrono>
#include <map>
#include <pthread.h>
#include <string>

namespace search_engine {

void PopFromScheduler::pop(
    PageScheduler &pageScheduler, std::string &url, std::string &baseUrl,
    bool &useLastCrawlEndTime,
    std::map<std::string, SiteAttributes> &siteAttributesMap,
    std::map<std::string, Crawl::timePoint> &lastCrawlEndTimeMap,
    pthread_mutex_t *memoryMutex) {
  url = pageScheduler.pop();
  baseUrl = utils::baseUrl(url);

  auto it = lastCrawlEndTimeMap.find(baseUrl);

  if (it == lastCrawlEndTimeMap.end()) {
    if (memoryMutex != NULL) {
      pthread_mutex_lock(memoryMutex);
    }

    siteAttributesMap.operator[](baseUrl) = SiteAttributes();
    lastCrawlEndTimeMap.operator[](baseUrl) = std::chrono::steady_clock::now();
    useLastCrawlEndTime = false;

    if (memoryMutex != NULL) {
      pthread_mutex_unlock(memoryMutex);
    }
  } else {
    useLastCrawlEndTime = true;
  }
}

} // namespace search_engine
