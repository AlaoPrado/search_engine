#include "SchedulerPopAllTask.hpp"
#include "../../../utils/SynchronizedQueue.hpp"
#include "../../../utils/Url.hpp"
#include "../action/Crawl.hpp"
#include "../action/PageStorage.hpp"
#include "CrawlTask.hpp"
#include <CkSpider.h>
#include <chrono>
#include <iostream>
#include <pthread.h>
#include <string>

namespace search_engine {

SchedulerPopAllTask::SchedulerPopAllTask(
    std::size_t numExpectedPops, pthread_mutex_t *memoryMutex,
    SynchonizedPageGroupScheduler *pageGroupScheduler, ThreadPool *crawlPool,
    utils::SynchronizedQueue<CkSpider> *spiderQueue,
    std::vector<std::string> *mustMatchPatterns,
    std::vector<std::string> *avoidPatterns,
    std::map<std::string, double> *totalTimeMap,
    std::map<std::string, Crawl::timePoint> *lastCrawlEndTimeMap,
    pthread_mutex_t *crawlMutex)
    : numExpectedPops(numExpectedPops), memoryMutex(memoryMutex),
      pageGroupScheduler(pageGroupScheduler), crawlPool(crawlPool),
      spiderQueue(spiderQueue), mustMatchPatterns(mustMatchPatterns),
      avoidPatterns(avoidPatterns), totalTimeMap(totalTimeMap),
      lastCrawlEndTimeMap(lastCrawlEndTimeMap), crawlMutex(crawlMutex) {}

SchedulerPopAllTask::~SchedulerPopAllTask() {}

void SchedulerPopAllTask::run() {
  for (size_t i = 0; i < numExpectedPops; i++) {

    // std::cout << "SchedulerPopAllTask pop await " + std::to_string(i)
              // << std::endl;

    std::string url = pageGroupScheduler->pop();

    // std::cout << "SchedulerPopAllTask " + url << std::endl;

    bool useLastCrawlEndTime = true;
    std::string baseUrl = utils::baseUrl(url);

    // std::cout << "SchedulerPopAllTask baseUrl " + baseUrl << std::endl;

    auto it = lastCrawlEndTimeMap->find(baseUrl);
    if (it == lastCrawlEndTimeMap->end()) {
      pthread_mutex_lock(memoryMutex);
      totalTimeMap->operator[](baseUrl) = 0;
      lastCrawlEndTimeMap->operator[](baseUrl) =
          std::chrono::steady_clock::now();
      pthread_mutex_unlock(memoryMutex);

      useLastCrawlEndTime = false;
    }

    // std::cout << "SchedulerPopAllTask new Spider " + url << std::endl;
    pthread_mutex_lock(memoryMutex);
    CkSpider *spider = new CkSpider();
    pthread_mutex_unlock(memoryMutex);

    // std::cout << "SchedulerPopAllTask time " + url << std::endl;

    double *totalTime = &(totalTimeMap->operator[](baseUrl));
    Crawl::timePoint *lastCrawlEndTime =
        &(lastCrawlEndTimeMap->operator[](baseUrl));

    pthread_mutex_lock(memoryMutex);
    CrawlTask *crawlTask = new CrawlTask(
        memoryMutex, spiderQueue, spider, url, mustMatchPatterns, avoidPatterns,
        totalTime, lastCrawlEndTime, useLastCrawlEndTime, crawlMutex);
    pthread_mutex_unlock(memoryMutex);

    // std::cout << "SchedulerPopAllTask add CrawlTask " + url << std::endl;
    crawlPool->addTask(crawlTask);
  }
}

} // namespace search_engine
