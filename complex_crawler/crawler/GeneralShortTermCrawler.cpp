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
#include "GeneralShortTermCrawler.hpp"
#include "../../threadpool/ThreadPool.hpp"
#include "../../utils/Assert.hpp"
#include "action/PushIntoScheduler.hpp"
#include "scheduler/sync/SynchonizedPageGroupScheduler.hpp"
#include "task/GeneralCrawlTask.hpp"
#include <iostream>
#include <pthread.h>

namespace search_engine {

GeneralShortTermCrawler::GeneralShortTermCrawler(std::string storageDirectory,
                                                 bool verbose, int numThreads)
    : Crawler(storageDirectory, verbose), numThreads(numThreads) {}

GeneralShortTermCrawler::~GeneralShortTermCrawler() {}

void GeneralShortTermCrawler::crawl(std::vector<std::string> &seedUrls,
                                    std::size_t numPagesToCrawl) {
  utils::assertTrue(numThreads > 0, "Error: numThreads < 0");

  pthread_mutex_t popMutex, storeMutex, failMutex, pushMutex, memoryMutex;

  pthread_mutex_init(&popMutex, 0);
  pthread_mutex_init(&storeMutex, 0);
  pthread_mutex_init(&pushMutex, 0);
  pthread_mutex_init(&memoryMutex, 0);
  pthread_mutex_init(&failMutex, 0);

  CounterFlag counterFlag(numThreads);
  ThreadPool generalCrawlPool(numThreads);
  GeneralCrawlTask generalCrawlTasks[numThreads];
  auto *pageScheduler = new SynchonizedPageGroupScheduler(&memoryMutex);
  int numPagesPushed;

  PushIntoScheduler::push(*pageScheduler, seedUrls, *(this->viewedUrls),
                          numPagesToCrawl, numPagesPushed);

  std::size_t numPagesThreads = numPagesToCrawl / numThreads;
  std::size_t numPagesFinalThread =
      numPagesThreads + numPagesToCrawl % numThreads;

  for (int i = 0; i < numThreads - 1; i++) {
    if (numPagesThreads > 0) {
      generalCrawlTasks[i].set(
          this->storageDirectory, &this->mustMatchPatterns,
          &this->avoidPatterns, numPagesThreads * i, numPagesThreads,
          &numPagesToCrawl, pageScheduler, this->viewedUrls,
          this->siteAttributesMap, this->lastCrawlEndTimeMap, &popMutex,
          &storeMutex, &pushMutex, &memoryMutex, &failMutex, &counterFlag, i);
      generalCrawlPool.addTask(&generalCrawlTasks[i]);
    }
  }

  if (numPagesFinalThread > 0) {
    generalCrawlTasks[numThreads - 1].set(
        this->storageDirectory, &this->mustMatchPatterns, &this->avoidPatterns,
        numPagesThreads * (numThreads - 1), numPagesFinalThread,
        &numPagesToCrawl, pageScheduler, this->viewedUrls,
        this->siteAttributesMap, this->lastCrawlEndTimeMap, &popMutex,
        &storeMutex, &pushMutex, &memoryMutex, &failMutex, &counterFlag,
        numThreads - 1);
    generalCrawlPool.addTask(&generalCrawlTasks[numThreads - 1]);
  }

  counterFlag.wait();

  this->printCrawlStatus();

  pthread_mutex_destroy(&popMutex);
  pthread_mutex_destroy(&storeMutex);
  pthread_mutex_destroy(&pushMutex);
  pthread_mutex_destroy(&memoryMutex);
  pthread_mutex_destroy(&failMutex);

  delete pageScheduler;
}

} // namespace search_engine
