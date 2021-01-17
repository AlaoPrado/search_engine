#include "SchedulerPushAllTask.hpp"
#include "../../../threadpool/CounterFlag.hpp"
#include "../../../threadpool/ThreadPool.hpp"
#include "../../../utils/Assert.hpp"
#include "../../../utils/SynchronizedQueue.hpp"
#include "../SiteAttributes.hpp"
#include "../action/PageStorage.hpp"
#include "../action/PushIntoScheduler.hpp"
#include "../scheduler/sync/SynchonizedPageGroupScheduler.hpp"
#include "CrawlTaskResult.hpp"
#include "SchedulerPopAllTask.hpp"
#include "StorePageTask.hpp"
#include <CkSpider.h>
#include <exception>
#include <iostream>
#include <map>
#include <pthread.h>
#include <string>

namespace search_engine {

SchedulerPushAllTask::SchedulerPushAllTask(
    CounterFlag *counterFlag, std::size_t numPagesToCrawl,
    pthread_mutex_t *memoryMutex,
    SynchonizedPageGroupScheduler *pageGroupScheduler, ThreadPool *crawlPool,
    utils::SynchronizedQueue<CrawlTaskResult> *crawlTaskResultQueue,
    std::vector<std::string> *mustMatchPatterns,
    std::vector<std::string> *avoidPatterns,
    std::map<std::string, SiteAttributes> *siteAttributesMap,
    std::map<std::string, Crawl::timePoint> *lastCrawlEndTimeMap,
    ThreadPool *schedulerPopPool, std::map<std::string, bool> *viewedUrls,
    std::string storageDirectory, ThreadPool *storePool, bool verbose)
    : counterFlag(counterFlag), numPagesToCrawl(numPagesToCrawl),
      memoryMutex(memoryMutex), pageGroupScheduler(pageGroupScheduler),
      crawlPool(crawlPool), crawlTaskResultQueue(crawlTaskResultQueue),
      mustMatchPatterns(mustMatchPatterns), avoidPatterns(avoidPatterns),
      siteAttributesMap(siteAttributesMap),
      lastCrawlEndTimeMap(lastCrawlEndTimeMap),
      schedulerPopPool(schedulerPopPool), viewedUrls(viewedUrls),
      storageDirectory(storageDirectory), storePool(storePool),
      verbose(verbose) {}

SchedulerPushAllTask::~SchedulerPushAllTask() {}

void SchedulerPushAllTask::run() {
  CounterFlag storeCounterFlag(0);
  size_t numCrawledPages = 0;
  while (numCrawledPages < numPagesToCrawl) {
    CrawlTaskResult *crawlTaskResult = crawlTaskResultQueue->pop();
    CkSpider *spider = crawlTaskResult->getSpider();
    try {
      std::string url = crawlTaskResult->getPage().getUrl();

      utils::assertTrue(crawlTaskResult->getSuccess(),
                        "Error: failed to crawl page" + url +
                            "trying another page");

      pageGroupScheduler->finishWork(crawlTaskResult->getPage().getUrl());
      storeCounterFlag.reset(1);

      bool storeSuccess;

      pthread_mutex_lock(memoryMutex);
      StorePageTask *storePageTask =
          new StorePageTask(&storeCounterFlag, storageDirectory, spider,
                            numCrawledPages, &storeSuccess);
      pthread_mutex_unlock(memoryMutex);

      storePool->addTask(storePageTask);
      PushIntoScheduler::push(
          pageGroupScheduler, *spider, viewedUrls, numPagesToCrawl,
          crawlTaskResult->getPage().getLevel(), memoryMutex);

      storeCounterFlag.wait();

      utils::assertTrue(storeSuccess, "Error: failed to store page" + url +
                                          ", trying another page");

      numCrawledPages++;
    } catch (std::exception &e) {
      const int numExtraPops = 1;
      pthread_mutex_lock(memoryMutex);
      SchedulerPopAllTask *schedulerPopAllTask = new SchedulerPopAllTask(
          numExtraPops, memoryMutex, pageGroupScheduler, crawlPool,
          crawlTaskResultQueue, mustMatchPatterns, avoidPatterns,
          siteAttributesMap, lastCrawlEndTimeMap);
      pthread_mutex_unlock(memoryMutex);
      schedulerPopPool->addTask(schedulerPopAllTask);
    }

    pthread_mutex_lock(memoryMutex);
    // std::cout << "SchedulerPushAllTask memory lock " << std::endl;
    delete spider;
    delete crawlTaskResult;
    // std::cout << "SchedulerPushAllTask memory unlock" << std::endl;
    pthread_mutex_unlock(memoryMutex);
  }

  counterFlag->signal();
}

} // namespace search_engine
