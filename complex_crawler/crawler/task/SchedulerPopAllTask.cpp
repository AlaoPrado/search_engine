#include "SchedulerPopAllTask.hpp"
#include "../../../utils/SynchronizedQueue.hpp"
#include "../../../utils/Url.hpp"
#include "../Page.hpp"
#include "../SiteAttributes.hpp"
#include "../action/Crawl.hpp"
#include "../action/PopFromScheduler.hpp"
#include "CrawlTask.hpp"
#include "CrawlTaskResult.hpp"
#include <CkSpider.h>
#include <chrono>
#include <iostream>
#include <pthread.h>
#include <string>

namespace search_engine {

SchedulerPopAllTask::SchedulerPopAllTask(
    std::size_t numExpectedPops, pthread_mutex_t *memoryMutex,
    SynchonizedPageGroupScheduler *pageGroupScheduler, ThreadPool *crawlPool,
    utils::SynchronizedQueue<CrawlTaskResult> *crawlTaskResultQueue,
    std::vector<std::string> *mustMatchPatterns,
    std::vector<std::string> *avoidPatterns,
    std::map<std::string, SiteAttributes> *siteAttributesMap,
    std::map<std::string, Crawl::timePoint> *lastCrawlEndTimeMap)
    : numExpectedPops(numExpectedPops), memoryMutex(memoryMutex),
      pageGroupScheduler(pageGroupScheduler), crawlPool(crawlPool),
      crawlTaskResultQueue(crawlTaskResultQueue),
      mustMatchPatterns(mustMatchPatterns), avoidPatterns(avoidPatterns),
      siteAttributesMap(siteAttributesMap),
      lastCrawlEndTimeMap(lastCrawlEndTimeMap) {}

SchedulerPopAllTask::~SchedulerPopAllTask() {}

void SchedulerPopAllTask::run() {
  for (size_t i = 0; i < numExpectedPops; i++) {
    Page page("");
    std::string baseUrl;
    bool useLastCrawlEndTime;

    PopFromScheduler::pop(*pageGroupScheduler, page, baseUrl,
                          useLastCrawlEndTime, *siteAttributesMap,
                          *lastCrawlEndTimeMap, memoryMutex);

    // std::cout << "SchedulerPopAllTask new Spider " + url << std::endl;
    pthread_mutex_lock(memoryMutex);
    CkSpider *spider = new CkSpider();
    pthread_mutex_unlock(memoryMutex);

    // std::cout << "SchedulerPopAllTask time " + url << std::endl;

    SiteAttributes *siteAttributes = &(siteAttributesMap->operator[](baseUrl));
    Crawl::timePoint *lastCrawlEndTime =
        &(lastCrawlEndTimeMap->operator[](baseUrl));

    pthread_mutex_lock(memoryMutex);
    CrawlTask *crawlTask = new CrawlTask(
        memoryMutex, crawlTaskResultQueue, spider, page, mustMatchPatterns,
        avoidPatterns, siteAttributes, lastCrawlEndTime, useLastCrawlEndTime);
    pthread_mutex_unlock(memoryMutex);

    // std::cout << "SchedulerPopAllTask add CrawlTask " + url << std::endl;
    crawlPool->addTask(crawlTask);
  }
}

} // namespace search_engine
