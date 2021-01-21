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

GeneralCrawlTask::GeneralCrawlTask()
    : storageDirectory(""), mustMatchPatterns(NULL), avoidPatterns(NULL),
      numPagesToCrawl(0), allNumPagesToCrawl(NULL), pageScheduler(NULL),
      viewedUrls(NULL), siteAttributesMap(NULL), lastCrawlEndTimeMap(NULL),
      popMutex(NULL), storeMutex(NULL), pushMutex(NULL), memoryMutex(NULL),
      failMutex(NULL), counterFlag(NULL), threadId(0) {}

GeneralCrawlTask::~GeneralCrawlTask() {}

void GeneralCrawlTask::set(
    std::string storageDirectory, std::vector<std::string> *mustMatchPatterns,
    std::vector<std::string> *avoidPatterns, std::size_t initialNumPageToCrawl,
    std::size_t numPagesToCrawl, std::size_t *allNumPagesToCrawl,
    SynchonizedPageGroupScheduler *pageScheduler,
    std::map<std::string, bool> *viewedUrls,
    std::map<std::string, SiteAttributes> *siteAttributesMap,
    std::map<std::string, Crawl::timePoint> *lastCrawlEndTimeMap,
    pthread_mutex_t *popMutex, pthread_mutex_t *storeMutex,
    pthread_mutex_t *pushMutex, pthread_mutex_t *memoryMutex,
    pthread_mutex_t *failMutex, CounterFlag *counterFlag, int threadId) {
  this->storageDirectory = storageDirectory;
  this->mustMatchPatterns = mustMatchPatterns;
  this->avoidPatterns = avoidPatterns;
  this->initialNumPageToCrawl = initialNumPageToCrawl;
  this->numPagesToCrawl = numPagesToCrawl;
  this->allNumPagesToCrawl = allNumPagesToCrawl;
  this->pageScheduler = pageScheduler;
  this->viewedUrls = viewedUrls;
  this->siteAttributesMap = siteAttributesMap;
  this->lastCrawlEndTimeMap = lastCrawlEndTimeMap;
  this->popMutex = popMutex;
  this->storeMutex = storeMutex;
  this->pushMutex = pushMutex;
  this->memoryMutex = memoryMutex;
  this->failMutex = failMutex;
  this->counterFlag = counterFlag;
  this->threadId = threadId;
}

void GeneralCrawlTask::run() {
  std::size_t numCrawledPages = 0;
  int numPagesPushed = 0;

  // std::cout << "Thread " << threadId << std::endl;
  while (numCrawledPages < numPagesToCrawl) {
    Page page("");
    std::string baseUrl;
    bool useLastCrawlEndTime;

    std::cout << "Await " + std::to_string(threadId) +
                     " size: " + std::to_string(pageScheduler->size())
              << std::endl;
    PopFromScheduler::pop(*pageScheduler, page, baseUrl, useLastCrawlEndTime,
                          *siteAttributesMap, *lastCrawlEndTimeMap, memoryMutex,
                          popMutex);
    std::cout << "Pop " + std::to_string(threadId) + " " + page.getUrl()
              << std::endl;

    CkSpider spider;
    SiteAttributes *siteAttributes = &(siteAttributesMap->operator[](baseUrl));
    Crawl::timePoint *lastCrawlEndTime =
        &(lastCrawlEndTimeMap->operator[](baseUrl));

    try {
      Crawl::crawlUrl(spider, page, *mustMatchPatterns, *avoidPatterns,
                      *siteAttributes, *lastCrawlEndTime, useLastCrawlEndTime);
      std::cout << "Finish " + std::to_string(threadId) + " " + page.getUrl()
                << std::endl;
      PageStorage::storePage(storageDirectory, spider,
                             initialNumPageToCrawl + numCrawledPages,
                             storeMutex);
      numCrawledPages++;
      std::cout << "Success: " + page.getUrl() << std::endl;
      // std::cout << "allNumPagesToCrawl: " << *allNumPagesToCrawl <<
      // std::endl;
      PushIntoScheduler::push(*pageScheduler, spider, *viewedUrls,
                              *allNumPagesToCrawl, page.getLevel(),
                              numPagesPushed, memoryMutex, pushMutex);

      if (page.getLevel() == 0) {
        siteAttributes->addNumPagesLevel1(numPagesPushed);
      }
      // std::cout << "numPagesPushed: " << numPagesPushed << std::endl;
    } catch (std::exception &e) {
      std::cout << "Error while crawling page " + page.getUrl() << std::endl;
      std::cout << e.what() << std::endl;

      pthread_mutex_lock(failMutex);
      (*allNumPagesToCrawl)++;
      pthread_mutex_unlock(failMutex);
    }
    pageScheduler->finishWork(page.getUrl());
  }

  counterFlag->signal();
}

} // namespace search_engine
