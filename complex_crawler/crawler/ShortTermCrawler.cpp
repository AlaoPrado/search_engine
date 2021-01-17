#include "ShortTermCrawler.hpp"
#include "../../threadpool/CounterFlag.hpp"
#include "../../threadpool/ThreadPool.hpp"
#include "../../utils/SynchronizedQueue.hpp"
#include "action/PushIntoScheduler.hpp"
#include "scheduler/sync/SynchonizedPageGroupScheduler.hpp"
#include "task/CrawlTask.hpp"
#include "task/CrawlTaskResult.hpp"
#include "task/SchedulerPopAllTask.hpp"
#include "task/SchedulerPushAllTask.hpp"
#include <CkSpider.h>
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

  pthread_mutex_init(memoryMutex, 0);

  auto *pageScheduler =
      new SynchonizedPageGroupScheduler(numPagesToCrawl, memoryMutex);
  auto *crawlTaskResultQueue =
      new utils::SynchronizedQueue<CrawlTaskResult>(memoryMutex);
  auto *schedulerPushAllPool = new ThreadPool(1, memoryMutex);
  auto *schedulerPopAllPool = new ThreadPool(1, memoryMutex);
  auto *storePool = new ThreadPool(1, memoryMutex);
  auto *crawlPool = new ThreadPool(this->numThreads, memoryMutex);

  PushIntoScheduler::push(pageScheduler, seedUrls, this->viewedUrls,
                          numPagesToCrawl, memoryMutex);

  CounterFlag *counterFlag = new CounterFlag(1);

  SchedulerPushAllTask *schedulerPushAllTask = new SchedulerPushAllTask(
      counterFlag, numPagesToCrawl, memoryMutex, crawlTaskResultQueue,
      pageScheduler, this->viewedUrls, this->storageDirectory, storePool,
      this->verbose);

  schedulerPushAllPool->addTask(schedulerPushAllTask);

  SchedulerPopAllTask schedulerPopAllTask(
      numPagesToCrawl, memoryMutex, pageScheduler, crawlPool,
      crawlTaskResultQueue, &mustMatchPatterns, &avoidPatterns,
      siteAttributesMap, lastCrawlEndTimeMap);

  schedulerPopAllTask.run();

  counterFlag->wait();

  this->printCrawlStatus();

  delete counterFlag;
  delete pageScheduler;
  delete crawlTaskResultQueue;
  delete schedulerPushAllPool;
  delete schedulerPopAllPool;
  delete storePool;
  delete crawlPool;

  pthread_mutex_destroy(memoryMutex);

  delete memoryMutex;
}

} // namespace search_engine
