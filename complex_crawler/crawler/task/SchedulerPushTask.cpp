#include "SchedulerPushTask.hpp"
#include "../action/PushIntoScheduler.hpp"
#include <CkSpider.h>

namespace search_engine {

SchedulerPushTask::SchedulerPushTask(pthread_mutex_t *memoryMutex,
                                     PageScheduler *pageScheduler,
                                     CrawlTaskResult *crawlTaskResult,
                                     std::map<std::string, bool> *viewedUrls,
                                     std::size_t numPagesToCrawl,
                                     CounterFlag *processCounterFlag)
    : memoryMutex(memoryMutex), pageScheduler(pageScheduler),
      crawlTaskResult(crawlTaskResult), viewedUrls(viewedUrls),
      numPagesToCrawl(numPagesToCrawl), processCounterFlag(processCounterFlag) {
}

SchedulerPushTask::~SchedulerPushTask() {}

void SchedulerPushTask::run() {
  CkSpider *spider = crawlTaskResult->getSpider();
  int numPagesPushed;

  PushIntoScheduler::push(*pageScheduler, *spider, *viewedUrls, numPagesToCrawl,
                          crawlTaskResult->getPage().getLevel(), numPagesPushed,
                          memoryMutex);
  processCounterFlag->wait();

  pthread_mutex_lock(memoryMutex);
  delete spider;
  delete crawlTaskResult;
  delete processCounterFlag;
  pthread_mutex_unlock(memoryMutex);
}

} // namespace search_engine
