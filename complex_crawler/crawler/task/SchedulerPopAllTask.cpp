/* Copyright 2021 Alan Prado Araújo

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
Author: alanpradoaraujo@gmail.com (Alan Prado Araújo) */
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
    pthread_mutex_t *memoryMutex, std::size_t numExpectedPops,
    SynchonizedPageGroupScheduler *pageGroupScheduler, ThreadPool *crawlPool,
    utils::SynchronizedQueue<CrawlTaskResult> *crawlTaskResultQueue,
    std::vector<std::string> *mustMatchPatterns,
    std::vector<std::string> *avoidPatterns,
    std::map<std::string, SiteAttributes> *siteAttributesMap,
    std::map<std::string, Crawl::timePoint> *lastCrawlEndTimeMap)
    : memoryMutex(memoryMutex), numExpectedPops(numExpectedPops),
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

    std::cout << "Scheduler pop await. Size: " +
                     std::to_string(pageGroupScheduler->size())
              << std::endl;
    PopFromScheduler::pop(*pageGroupScheduler, page, baseUrl,
                          useLastCrawlEndTime, *siteAttributesMap,
                          *lastCrawlEndTimeMap, memoryMutex);
    std::cout << "Scheduler pop url: " + page.getUrl() << std::endl;

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
