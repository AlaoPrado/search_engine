#include "SchedulerPushAllTask.hpp"
#include "../../../threadpool/CounterFlag.hpp"
#include "../../../threadpool/ThreadPool.hpp"
#include "../../../utils/SynchronizedQueue.hpp"
#include "../action/PageStorage.hpp"
#include "../action/PushIntoScheduler.hpp"
#include "../scheduler/sync/SynchonizedPageGroupScheduler.hpp"
#include "CrawlTaskResult.hpp"
#include "StorePageTask.hpp"
#include <CkSpider.h>
#include <iostream>
#include <map>
#include <pthread.h>
#include <string>

namespace search_engine {

SchedulerPushAllTask::SchedulerPushAllTask(
    CounterFlag *counterFlag, std::size_t numPagesToCrawl,
    pthread_mutex_t *memoryMutex,
    utils::SynchronizedQueue<CrawlTaskResult> *crawlTaskResultQueue,
    SynchonizedPageGroupScheduler *pageGroupScheduler,
    std::map<std::string, bool> *viewedUrls, std::string storageDirectory,
    ThreadPool *storePool, bool verbose)
    : counterFlag(counterFlag), numPagesToCrawl(numPagesToCrawl),
      memoryMutex(memoryMutex), crawlTaskResultQueue(crawlTaskResultQueue),
      pageGroupScheduler(pageGroupScheduler), viewedUrls(viewedUrls),
      storageDirectory(storageDirectory), storePool(storePool),
      verbose(verbose) {}

SchedulerPushAllTask::~SchedulerPushAllTask() {}

void SchedulerPushAllTask::run() {
  CounterFlag storeCounterFlag(0);
  for (size_t i = 0; i < numPagesToCrawl; i++) {
    CrawlTaskResult *crawlTaskResult = crawlTaskResultQueue->pop();
    CkSpider *spider = crawlTaskResult->getSpider();

    pageGroupScheduler->finishWork(crawlTaskResult->getPage().getUrl());

    storeCounterFlag.reset(1);

    // PageStorage::storePage(storageDirectory, *spider, i);

    pthread_mutex_lock(memoryMutex);
    StorePageTask *storePageTask =
        new StorePageTask(&storeCounterFlag, storageDirectory, spider, i);
    pthread_mutex_unlock(memoryMutex);

    storePool->addTask(storePageTask);

    // std::cout << "SchedulerPushAllTask push" << std::endl;

    PushIntoScheduler::push(pageGroupScheduler, *spider, viewedUrls,
                            numPagesToCrawl, memoryMutex);

    storeCounterFlag.wait();

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
