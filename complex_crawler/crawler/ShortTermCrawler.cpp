#include "ShortTermCrawler.hpp"
#include "../../threadpool/CounterFlag.hpp"
#include "../../threadpool/ThreadPool.hpp"
#include "../../utils/SynchronizedQueue.hpp"
#include "../../utils/Url.hpp"
#include "SiteAttributes.hpp"
#include "action/Crawl.hpp"
#include "action/PageStorage.hpp"
#include "action/PushIntoScheduler.hpp"
#include "scheduler/sync/SynchonizedPageGroupScheduler.hpp"
#include "task/CrawlTask.hpp"
#include "task/SchedulerPopAllTask.hpp"
#include "task/SchedulerPushAllTask.hpp"
#include <CkSpider.h>
#include <chrono>
#include <iostream>
#include <map>
#include <pthread.h>
#include <string>
#include <vector>

namespace search_engine {

ShortTermCrawler::ShortTermCrawler(std::string storageDirectory, bool verbose,
                                   int numThreads)
    : Crawler(storageDirectory, verbose), numThreads(numThreads) {}

ShortTermCrawler::~ShortTermCrawler() {}

void ShortTermCrawler::crawl(std::vector<std::string> &seedUrls,
                             std::size_t numPagesToCrawl) {

  pthread_mutex_t *memoryMutex = new pthread_mutex_t();
  pthread_mutex_t *crawlMutex = new pthread_mutex_t();

  pthread_mutex_init(memoryMutex, 0);
  pthread_mutex_init(crawlMutex, 0);

  SynchonizedPageGroupScheduler *pageGroupScheduler =
      new SynchonizedPageGroupScheduler(numPagesToCrawl, memoryMutex);

  this->pageScheduler = (PageScheduler *)pageGroupScheduler;
  auto *siteAttributesMap = new std::map<std::string, SiteAttributes>();
  auto *lastCrawlEndTimeMap = new std::map<std::string, Crawl::timePoint>();
  auto *spiderQueue = new utils::SynchronizedQueue<CkSpider>(memoryMutex);
  auto *schedulerPushAllPool = new ThreadPool(1, memoryMutex);
  auto *schedulerPopAllPool = new ThreadPool(1, memoryMutex);
  auto *storePool = new ThreadPool(1, memoryMutex);
  auto *crawlPool = new ThreadPool(this->numThreads, memoryMutex);

  PushIntoScheduler::push(this->pageScheduler, seedUrls, this->viewedUrls,
                          numPagesToCrawl, memoryMutex);

  CounterFlag *counterFlag = new CounterFlag(1);

  SchedulerPushAllTask *schedulerPushAllTask = new SchedulerPushAllTask(
      counterFlag, numPagesToCrawl, memoryMutex, spiderQueue,
      pageGroupScheduler, this->viewedUrls, this->storageDirectory, storePool,
      this->verbose);

  schedulerPushAllPool->addTask(schedulerPushAllTask);

  SchedulerPopAllTask schedulerPopAllTask(
      numPagesToCrawl, memoryMutex, pageGroupScheduler, crawlPool, spiderQueue,
      &mustMatchPatterns, &avoidPatterns, siteAttributesMap,
      lastCrawlEndTimeMap, crawlMutex);

  schedulerPopAllTask.run();

  counterFlag->wait();

  if (verbose) {
    for (auto it = siteAttributesMap->begin(); it != siteAttributesMap->end();
         it++) {
      std::cout << "Web site: " << it->first << std::endl;
      std::cout << "Number of URLs at level 1 crawled: "
                << it->second.getNumPagesLeve1() << std::endl;
      std::cout << "Average crawl time for (milliseconds): "
                << it->second.getAverageTime() << std::endl;
      std::cout << "Average page size (Bytes): "
                << it->second.getAveragePageSize() << std::endl;
      std::cout << std::endl;
    }
  }

  delete counterFlag;
  delete this->pageScheduler;
  delete siteAttributesMap;
  delete lastCrawlEndTimeMap;
  delete spiderQueue;
  delete schedulerPushAllPool;
  delete schedulerPopAllPool;
  delete storePool;
  delete crawlPool;

  pthread_mutex_destroy(memoryMutex);
  pthread_mutex_destroy(crawlMutex);

  delete memoryMutex;
  delete crawlMutex;
}

} // namespace search_engine
