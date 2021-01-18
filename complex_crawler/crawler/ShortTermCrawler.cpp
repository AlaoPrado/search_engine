#include "ShortTermCrawler.hpp"
#include "../../threadpool/CounterFlag.hpp"
#include "../../threadpool/ThreadPool.hpp"
#include "../../utils/SynchronizedQueue.hpp"
#include "action/PushIntoScheduler.hpp"
#include "scheduler/sync/SynchonizedPageGroupScheduler.hpp"
#include "task/CrawlTask.hpp"
#include "task/CrawlTaskResult.hpp"
#include "task/ProcessCrawlResultsTask.hpp"
#include "task/SchedulerPopAllTask.hpp"
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

  auto *pageScheduler = new SynchonizedPageGroupScheduler(memoryMutex);
  auto *crawlTaskResultQueue =
      new utils::SynchronizedQueue<CrawlTaskResult>(memoryMutex);
  auto *schedulerPushPool = new ThreadPool(1, memoryMutex);
  auto *schedulerPopPool = new ThreadPool(1, memoryMutex);
  auto *storePool = new ThreadPool(1, memoryMutex);
  auto *crawlPool = new ThreadPool(this->numThreads, memoryMutex);
  int numPagesPushed;
  
  PushIntoScheduler::push(*pageScheduler, seedUrls, *(this->viewedUrls),
                          numPagesToCrawl, numPagesPushed, memoryMutex);

  CounterFlag *counterFlag = new CounterFlag(1);

  SchedulerPopAllTask *schedulerPopAllTask = new SchedulerPopAllTask(
      memoryMutex, numPagesToCrawl, pageScheduler, crawlPool,
      crawlTaskResultQueue, &mustMatchPatterns, &avoidPatterns,
      siteAttributesMap, lastCrawlEndTimeMap);

  ProcessCrawlResultsTask *processCrawlResultsTask =
      new ProcessCrawlResultsTask(
          counterFlag, memoryMutex, numPagesToCrawl, pageScheduler, crawlPool,
          crawlTaskResultQueue, &mustMatchPatterns, &avoidPatterns,
          siteAttributesMap, lastCrawlEndTimeMap, schedulerPopPool,
          this->viewedUrls, this->storageDirectory, storePool);

  schedulerPopPool->addTask(schedulerPopAllTask);
  schedulerPushPool->addTask(processCrawlResultsTask);

  counterFlag->wait();

  this->printCrawlStatus();

  delete counterFlag;
  delete pageScheduler;
  delete crawlTaskResultQueue;
  delete schedulerPushPool;
  delete schedulerPopPool;
  delete storePool;
  delete crawlPool;

  pthread_mutex_destroy(memoryMutex);

  delete memoryMutex;
}

} // namespace search_engine
