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
#pragma once

#include "../../../threadpool/CounterFlag.hpp"
#include "../../../threadpool/Task.hpp"
#include "../../../threadpool/ThreadPool.hpp"
#include "../../../utils/SynchronizedQueue.hpp"
#include "../SiteAttributes.hpp"
#include "../action/Crawl.hpp"
#include "../scheduler/sync/SynchonizedPageGroupScheduler.hpp"
#include "CrawlTaskResult.hpp"
#include <map>
#include <pthread.h>
#include <string>

namespace search_engine {

class ProcessCrawlResultsTask : public Task {
private:
  CounterFlag *counterFlag;
  pthread_mutex_t *memoryMutex;
  std::size_t numPagesToCrawlNext, numPagesToCrawl, numCrawledPagesUntilNow;
  SynchonizedPageGroupScheduler *pageGroupScheduler;
  ThreadPool *crawlPool;
  utils::SynchronizedQueue<CrawlTaskResult> *crawlTaskResultQueue;
  std::vector<std::string> *mustMatchPatterns;
  std::vector<std::string> *avoidPatterns;
  std::map<std::string, SiteAttributes> *siteAttributesMap;
  std::map<std::string, Crawl::timePoint> *lastCrawlEndTimeMap;
  ThreadPool *schedulerPopPool;
  std::map<std::string, bool> *viewedUrls;
  std::string storageDirectory;
  ThreadPool *storePool;
  ThreadPool *pushPool;

public:
  ProcessCrawlResultsTask(
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
      std::string storageDirectory, ThreadPool *storePool);
  ~ProcessCrawlResultsTask();
  void run() override;
};

} // namespace search_engine
