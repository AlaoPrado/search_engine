/* Copyright 2021 Alan Prado Araújo

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
Author: alanpradoaraujo@gmail.com (Alan Prado Araújo) */
#include "SchedulerPushTask.hpp"
#include "../action/PushIntoScheduler.hpp"
#include <CkSpider.h>

namespace search_engine {

SchedulerPushTask::SchedulerPushTask(CounterFlag *pushCounterFlag,
                                     pthread_mutex_t *memoryMutex,
                                     PageScheduler *pageScheduler,
                                     CrawlTaskResult *crawlTaskResult,
                                     std::map<std::string, bool> *viewedUrls,
                                     std::size_t numPagesToCrawl,
                                     CounterFlag *processCounterFlag)
    : pushCounterFlag(pushCounterFlag), memoryMutex(memoryMutex),
      pageScheduler(pageScheduler), crawlTaskResult(crawlTaskResult),
      viewedUrls(viewedUrls), numPagesToCrawl(numPagesToCrawl),
      processCounterFlag(processCounterFlag) {}

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

  pushCounterFlag->signal();
}

} // namespace search_engine
