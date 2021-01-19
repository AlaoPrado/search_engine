#include "ProcessCrawlResultsTask.hpp"
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
#include "SchedulerPushTask.hpp"
#include "StorePageTask.hpp"
#include <CkSpider.h>
#include <exception>
#include <iostream>
#include <map>
#include <pthread.h>
#include <string>

namespace search_engine {

ProcessCrawlResultsTask::ProcessCrawlResultsTask(
    CounterFlag *counterFlag, pthread_mutex_t *memoryMutex,
    std::size_t numPagesToCrawlNext, std::size_t numPagesToCrawl,
    std::size_t numCrawledPagesUntilNow,
    SynchonizedPageGroupScheduler *pageGroupScheduler, ThreadPool *crawlPool,
    utils::SynchronizedQueue<CrawlTaskResult> *crawlTaskResultQueue,
    std::vector<std::string> *mustMatchPatterns,
    std::vector<std::string> *avoidPatterns,
    std::map<std::string, SiteAttributes> *siteAttributesMap,
    std::map<std::string, Crawl::timePoint> *lastCrawlEndTimeMap,
    ThreadPool *schedulerPopPool, std::map<std::string, bool> *viewedUrls,
    std::string storageDirectory, ThreadPool *storePool, ThreadPool *pushPool)
    : counterFlag(counterFlag), memoryMutex(memoryMutex),
      numPagesToCrawlNext(numPagesToCrawlNext),
      numPagesToCrawl(numPagesToCrawl),
      numCrawledPagesUntilNow(numCrawledPagesUntilNow),
      pageGroupScheduler(pageGroupScheduler), crawlPool(crawlPool),
      crawlTaskResultQueue(crawlTaskResultQueue),
      mustMatchPatterns(mustMatchPatterns), avoidPatterns(avoidPatterns),
      siteAttributesMap(siteAttributesMap),
      lastCrawlEndTimeMap(lastCrawlEndTimeMap),
      schedulerPopPool(schedulerPopPool), viewedUrls(viewedUrls),
      storageDirectory(storageDirectory), storePool(storePool),
      pushPool(pushPool) {}

ProcessCrawlResultsTask::~ProcessCrawlResultsTask() {}

void ProcessCrawlResultsTask::run() {
  size_t numCrawledPages = 0;
  CounterFlag storeCounterFlag(1);

  while (numCrawledPages < numPagesToCrawlNext) {
    std::cout << "Scheduler await spider" << std::endl;
    CrawlTaskResult *crawlTaskResult = crawlTaskResultQueue->pop();
    CkSpider *spider = crawlTaskResult->getSpider();

    try {
      std::string url = crawlTaskResult->getPage().getUrl();
      pageGroupScheduler->finishWork(url);
      std::cout << "Scheduler pop spider: " + url << std::endl;

      utils::assertTrue(crawlTaskResult->getSuccess(),
                        "Error: failed to crawl page " + url +
                            ", trying another page");

      storeCounterFlag.reset(1);

      bool storeSuccess;

      pthread_mutex_lock(memoryMutex);

      StorePageTask *storePageTask = new StorePageTask(
          &storeCounterFlag, storageDirectory, spider,
          numCrawledPagesUntilNow + numCrawledPages, &storeSuccess);
      CounterFlag *processCounterFlag = new CounterFlag(1);
      SchedulerPushTask *schedulerPushTask = new SchedulerPushTask(
          memoryMutex, pageGroupScheduler, crawlTaskResult, viewedUrls,
          numPagesToCrawl, processCounterFlag);
      pthread_mutex_unlock(memoryMutex);

      storePool->addTask(storePageTask);
      pushPool->addTask(schedulerPushTask);

      storeCounterFlag.wait();
      processCounterFlag->signal();

      utils::assertTrue(storeSuccess, "Error: failed to store page " + url +
                                          ", trying another page");

      std::cout << "Success: " + url << std::endl;

      numCrawledPages++;
    } catch (std::exception &e) {
      std::cout << "Error while crawling page " +
                       crawlTaskResult->getPage().getUrl()
                << std::endl;
      std::cout << e.what() << std::endl;

      const int numExtraPops = 1;

      pthread_mutex_lock(memoryMutex);
      SchedulerPopAllTask *schedulerPopAllTask = new SchedulerPopAllTask(
          memoryMutex, numExtraPops, pageGroupScheduler, crawlPool,
          crawlTaskResultQueue, mustMatchPatterns, avoidPatterns,
          siteAttributesMap, lastCrawlEndTimeMap);
      pthread_mutex_unlock(memoryMutex);

      schedulerPopPool->addTask(schedulerPopAllTask);
    }
  }

  counterFlag->signal();
}

} // namespace search_engine
