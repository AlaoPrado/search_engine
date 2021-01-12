#include "SchedulerPopAllTask.hpp"
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
    utils::SynchronizedQueue<CkSpider> *queue,
    std::vector<std::string> *mustMatchPatterns,
    std::vector<std::string> *avoidPatterns,
    std::map<std::string, double> *totalTimeMap,
    std::map<std::string, Crawl::timePoint> *lastCrawlEndTimeMap)
    : numExpectedPops(numExpectedPops), memoryMutex(memoryMutex),
      pageGroupScheduler(pageGroupScheduler), crawlPool(crawlPool),
      queue(queue), mustMatchPatterns(mustMatchPatterns),
      avoidPatterns(avoidPatterns), totalTimeMap(totalTimeMap),
      lastCrawlEndTimeMap(lastCrawlEndTimeMap) {}

SchedulerPopAllTask::~SchedulerPopAllTask() {}

void SchedulerPopAllTask::run() {
  for (size_t i = 0; i < numExpectedPops; i++) {
    std::string url = pageGroupScheduler->pop();

    std::cout << "SchedulerPopAllTask " + url << std::endl;

    bool useLastCrawlEndTime = true;

    std::cout << "SchedulerPopAllTask baseUrl"<< std::endl;

    std::string baseUrl = utils::baseUrl(url);
    auto it = lastCrawlEndTimeMap->find(baseUrl);
    if (it == lastCrawlEndTimeMap->end()) {
      pthread_mutex_lock(memoryMutex);
      totalTimeMap->operator[](baseUrl) = 0;
      lastCrawlEndTimeMap->operator[](baseUrl) =
          std::chrono::steady_clock::now();
      pthread_mutex_unlock(memoryMutex);
      useLastCrawlEndTime = false;
    }

    std::cout << "SchedulerPopAllTask new Spider"<< std::endl;
    pthread_mutex_lock(memoryMutex);
    CkSpider *spider = new CkSpider();
    pthread_mutex_unlock(memoryMutex);

    std::cout << "SchedulerPopAllTask time"<< std::endl;

    double *totalTime = &(totalTimeMap->operator[](baseUrl));
    Crawl::timePoint *lastCrawlEndTime =
        &(lastCrawlEndTimeMap->operator[](baseUrl));

    pthread_mutex_lock(memoryMutex);
    CrawlTask *crawlTask =
        new CrawlTask(queue, spider, url, mustMatchPatterns, avoidPatterns,
                      totalTime, lastCrawlEndTime, useLastCrawlEndTime);
    pthread_mutex_unlock(memoryMutex);

    std::cout << "SchedulerPopAllTask addTask"<< std::endl;
    crawlPool->addTask(crawlTask);
  }
}

} // namespace search_engine
