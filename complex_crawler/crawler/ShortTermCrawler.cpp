#include "ShortTermCrawler.hpp"
#include "../../threadpool/ThreadPool.hpp"
#include "../../utils/SynchronizedQueue.hpp"
#include "../../utils/Url.hpp"
#include "action/Crawl.hpp"
#include "action/PushIntoScheduler.hpp"
#include "scheduler/sync/SynchonizedPageGroupScheduler.hpp"
#include "task/CrawlTask.hpp"
#include "task/SchedulerPopAllTask.hpp"
#include "task/StorePageTask.hpp"
#include <CkSpider.h>
#include <chrono>
#include <iostream>
#include <map>
#include <pthread.h>
#include <string>

namespace search_engine {

ShortTermCrawler::ShortTermCrawler(std::string storageDirectory, bool verbose,
                                   int numThreads)
    : Crawler(storageDirectory, verbose), numThreads(numThreads) {}

ShortTermCrawler::~ShortTermCrawler() {}

void ShortTermCrawler::crawl(std::vector<std::string> &seedUrls,
                             std::size_t numPagesToCrawl) {

  pthread_mutex_t memoryMutex;

  pthread_mutex_init(&memoryMutex, 0);

  SynchonizedPageGroupScheduler *pageGroupScheduler =
      new SynchonizedPageGroupScheduler(numPagesToCrawl, &memoryMutex);

  this->pageScheduler = (PageScheduler *)pageGroupScheduler;
  auto *totalTimeMap = new std::map<std::string, double>();
  auto *lastCrawlEndTimeMap = new std::map<std::string, Crawl::timePoint>();
  auto *spiderQueue = new utils::SynchronizedQueue<CkSpider>(&memoryMutex);
  auto *pageGroupSchedulerPoperPool = new ThreadPool(1, &memoryMutex);
  auto *storePagePool = new ThreadPool(1, &memoryMutex);
  auto *crawlPool = new ThreadPool(this->numThreads, &memoryMutex);

  pageGroupSchedulerPoperPool->addTask(new SchedulerPopAllTask(
      numPagesToCrawl, &memoryMutex, pageGroupScheduler, crawlPool, spiderQueue,
      &mustMatchPatterns, &avoidPatterns, totalTimeMap, lastCrawlEndTimeMap));

  PushIntoScheduler::push(this->pageScheduler, seedUrls, this->viewedUrls,
                          numPagesToCrawl, &memoryMutex);

  for (std::size_t i = 0; i < numPagesToCrawl; i++) {
    CkSpider *spider = spiderQueue->pop();

    pageGroupScheduler->finishWork(spider->lastUrl());

    if (verbose) {
      std::cout << spider->lastUrl() << std::endl;
      std::cout << spider->get_NumUnspidered() << std::endl;
    }

    PushIntoScheduler::push(this->pageScheduler, *spider, this->viewedUrls,
                            numPagesToCrawl, &memoryMutex);
    std::cout << "ShortTerm push" << std::endl;

    pthread_mutex_lock(&memoryMutex);
    delete spider;
    pthread_mutex_unlock(&memoryMutex);
    std::cout << "ShortTerm delete" << std::endl;
  }

  delete this->pageScheduler;
  delete totalTimeMap;
  delete lastCrawlEndTimeMap;
  delete spiderQueue;
  delete pageGroupSchedulerPoperPool;
  delete storePagePool;
  delete crawlPool;

  pthread_mutex_destroy(&memoryMutex);
}

} // namespace search_engine
