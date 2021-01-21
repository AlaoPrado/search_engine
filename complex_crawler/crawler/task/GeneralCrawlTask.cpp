#include "GeneralCrawlTask.hpp"
#include "../Page.hpp"
#include "../SiteAttributes.hpp"
#include "../action/Crawl.hpp"
#include "../action/PageStorage.hpp"
#include "../action/PopFromScheduler.hpp"
#include "../action/PushIntoScheduler.hpp"
#include "../scheduler/PageScheduler.hpp"
#include <CkSpider.h>
#include <cstddef>
#include <iostream>
#include <map>
#include <string>
#include <vector>

namespace search_engine {

GeneralCrawlTask::GeneralCrawlTask() {}

GeneralCrawlTask::~GeneralCrawlTask() {}

void GeneralCrawlTask::set(
    std::string storageDirectory, std::vector<std::string> *mustMatchPatterns,
    std::vector<std::string> *avoidPatterns, std::size_t numPagesToCrawl,
    PageScheduler *pageScheduler, std::map<std::string, bool> *viewedUrls,
    std::map<std::string, SiteAttributes> *siteAttributesMap,
    std::map<std::string, Crawl::timePoint> *lastCrawlEndTimeMap,
    pthread_mutex_t *popMutex, pthread_mutex_t *storeMutex,
    pthread_mutex_t *pushMutex, pthread_mutex_t *memoryMutex) {
  this->storageDirectory = storageDirectory;
  this->mustMatchPatterns = mustMatchPatterns;
  this->avoidPatterns = avoidPatterns;
  this->numPagesToCrawl = numPagesToCrawl;
  this->pageScheduler = pageScheduler;
  this->viewedUrls = viewedUrls;
  this->siteAttributesMap = siteAttributesMap;
  this->lastCrawlEndTimeMap = lastCrawlEndTimeMap;
  this->popMutex = popMutex;
  this->storeMutex = storeMutex;
  this->pushMutex = pushMutex;
  this->memoryMutex = memoryMutex;
}

void GeneralCrawlTask::run() {
  std::size_t numCrawledPages = 0;
  int numPagesPushed = 0;

  while (numCrawledPages < numPagesToCrawl) {
    Page page("");
    std::string baseUrl;
    bool useLastCrawlEndTime;

    PopFromScheduler::pop(*pageScheduler, page, baseUrl, useLastCrawlEndTime,
                          *siteAttributesMap, *lastCrawlEndTimeMap, popMutex);

    CkSpider spider;
    SiteAttributes *siteAttributes = &(siteAttributesMap->operator[](baseUrl));
    Crawl::timePoint *lastCrawlEndTime =
        &(lastCrawlEndTimeMap->operator[](baseUrl));

    try {
      Crawl::crawlUrl(spider, page, *mustMatchPatterns, *avoidPatterns,
                      *siteAttributes, *lastCrawlEndTime, useLastCrawlEndTime);
      PageStorage::storePage(storageDirectory, spider, numCrawledPages,
                             storeMutex);
      numCrawledPages++;
      PushIntoScheduler::push(*pageScheduler, spider, *viewedUrls,
                              numPagesToCrawl, page.getLevel(), numPagesPushed,
                              memoryMutex, pushMutex);
    } catch (std::exception &e) {
      std::cout << "Error while crawling page " + page.getUrl() << std::endl;
      std::cout << e.what() << std::endl;
    }
  }
}

} // namespace search_engine
