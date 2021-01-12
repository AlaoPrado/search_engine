#include "ShortTermCrawler.hpp"
#include "../../threadpool/CounterFlag.hpp"
#include "../../threadpool/ThreadPool.hpp"
#include "../../utils/SynchronizedQueue.hpp"
#include "../../utils/Url.hpp"
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
  auto *totalTimeMap = new std::map<std::string, double>();
  auto *lastCrawlEndTimeMap = new std::map<std::string, Crawl::timePoint>();
  auto *spiderQueue = new utils::SynchronizedQueue<CkSpider>(memoryMutex);
  auto *schedulerPushAllPool = new ThreadPool(1, memoryMutex);
  auto *schedulerPopAllPool = new ThreadPool(1, memoryMutex);
  auto *crawlPool = new ThreadPool(this->numThreads, memoryMutex);

  // auto spiderToDelete = new std::vector<CkSpider *>();

  PushIntoScheduler::push(this->pageScheduler, seedUrls, this->viewedUrls,
                          numPagesToCrawl, memoryMutex);

  CounterFlag *counterFlag = new CounterFlag(1);

  SchedulerPushAllTask *schedulerPushAllTask = new SchedulerPushAllTask(
      counterFlag, numPagesToCrawl, memoryMutex, spiderQueue,
      pageGroupScheduler, this->viewedUrls, this->storageDirectory,
      this->verbose);

  schedulerPushAllPool->addTask(schedulerPushAllTask);

  SchedulerPopAllTask schedulerPopAllTask(
      numPagesToCrawl, memoryMutex, pageGroupScheduler, crawlPool, spiderQueue,
      &mustMatchPatterns, &avoidPatterns, totalTimeMap, lastCrawlEndTimeMap,
      crawlMutex);

  schedulerPopAllTask.run();

  counterFlag->wait();

  std::cout << "delete all begin " << std::endl;
  delete counterFlag;
  std::cout << "delete counterFlag " << std::endl;
  delete this->pageScheduler;
  std::cout << "delete pageScheduler " << std::endl;
  delete totalTimeMap;
  std::cout << "delete totalTimeMap " << std::endl;
  delete lastCrawlEndTimeMap;
  std::cout << "delete lastCrawlEndTimeMap " << std::endl;
  delete spiderQueue;
  std::cout << "delete spiderQueue " << std::endl;
  delete schedulerPushAllPool;
  delete schedulerPopAllPool;
  std::cout << "delete schedulerPopAllPool " << std::endl;
  delete crawlPool;
  std::cout << "delete crawlPool " << std::endl;

  pthread_mutex_destroy(memoryMutex);
  pthread_mutex_destroy(crawlMutex);

  delete memoryMutex;
  std::cout << "delete memoryMutex " << std::endl;
  delete crawlMutex;
  std::cout << "delete crawlMutex " << std::endl;
}

} // namespace search_engine
