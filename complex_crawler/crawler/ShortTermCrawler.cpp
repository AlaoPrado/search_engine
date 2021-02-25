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
#include <iostream>
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
  auto *processCrawlPool = new ThreadPool(1, memoryMutex);
  auto *schedulerPopPool = new ThreadPool(1, memoryMutex);
  auto *storePool = new ThreadPool(1, memoryMutex);
  auto *crawlPool = new ThreadPool(this->numThreads, memoryMutex);
  int numPagesPushed;

  PushIntoScheduler::push(*pageScheduler, seedUrls, *(this->viewedUrls),
                          numPagesToCrawl, numPagesPushed, memoryMutex);

  CounterFlag *counterFlag = new CounterFlag(1);

  int numPagesToCrawlMaxBatch = 1000;
  int numPagesToCrawlNext;
  int numPagesToCrawlInt = (int)numPagesToCrawl;

  for (int numPagesCrawled = 0; numPagesCrawled < numPagesToCrawlInt;
       numPagesCrawled += numPagesToCrawlMaxBatch) {
    if (numPagesCrawled > numPagesToCrawlInt - numPagesToCrawlMaxBatch) {
      numPagesToCrawlNext = numPagesToCrawlInt - numPagesCrawled;
    } else {
      numPagesToCrawlNext = numPagesToCrawlMaxBatch;
    }

    counterFlag->reset(1);

    SchedulerPopAllTask *schedulerPopAllTask = new SchedulerPopAllTask(
        memoryMutex, (std::size_t)numPagesToCrawlNext, pageScheduler, crawlPool,
        crawlTaskResultQueue, &mustMatchPatterns, &avoidPatterns,
        siteAttributesMap, lastCrawlEndTimeMap);

    ProcessCrawlResultsTask *processCrawlResultsTask =
        new ProcessCrawlResultsTask(
            counterFlag, memoryMutex, (std::size_t)numPagesToCrawlNext,
            numPagesToCrawl, (std::size_t)numPagesCrawled, pageScheduler,
            crawlPool, crawlTaskResultQueue, &mustMatchPatterns, &avoidPatterns,
            siteAttributesMap, lastCrawlEndTimeMap, schedulerPopPool,
            this->viewedUrls, this->storageDirectory, storePool);

    schedulerPopPool->addTask(schedulerPopAllTask);
    processCrawlPool->addTask(processCrawlResultsTask);

    counterFlag->wait();
  }

  this->printCrawlStatus();

  delete counterFlag;
  delete pageScheduler;
  delete crawlTaskResultQueue;
  delete processCrawlPool;
  delete schedulerPopPool;
  delete storePool;
  delete crawlPool;

  pthread_mutex_destroy(memoryMutex);

  delete memoryMutex;
}

} // namespace search_engine
