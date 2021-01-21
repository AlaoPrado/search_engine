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
