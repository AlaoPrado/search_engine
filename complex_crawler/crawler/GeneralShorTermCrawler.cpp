#include "GeneralShorTermCrawler.hpp"
#include "../../threadpool/ThreadPool.hpp"
#include "../../utils/Assert.hpp"
#include "action/PushIntoScheduler.hpp"
#include "scheduler/sync/SynchonizedPageGroupScheduler.hpp"
#include "task/GeneralCrawlTask.hpp"
#include <pthread.h>

namespace search_engine {

GeneralShorTermCrawler::GeneralShorTermCrawler(std::string storageDirectory,
                                               bool verbose, int numThreads)
    : Crawler(storageDirectory, verbose), numThreads(numThreads) {}

GeneralShorTermCrawler::~GeneralShorTermCrawler() {}

void GeneralShorTermCrawler::crawl(std::vector<std::string> &seedUrls,
                                   std::size_t numPagesToCrawl) {
  utils::assertTrue(numThreads > 0, "Error: numThreads < 0");

  pthread_mutex_t popMutex, storeMutex, pushMutex, memoryMutex;

  pthread_mutex_init(&popMutex, 0);
  pthread_mutex_init(&storeMutex, 0);
  pthread_mutex_init(&pushMutex, 0);
  pthread_mutex_init(&memoryMutex, 0);

  ThreadPool generalCrawlPool(numThreads);
  GeneralCrawlTask *generalCrawlTasks = new GeneralCrawlTask[numThreads];
  auto *pageScheduler = new SynchonizedPageGroupScheduler(&memoryMutex);
  int numPagesPushed;

  PushIntoScheduler::push(*pageScheduler, seedUrls, *(this->viewedUrls),
                          numPagesToCrawl, numPagesPushed);

  std::size_t numPagesFinalThreads = numPagesToCrawl / numThreads;
  std::size_t numPagesFirstThread =
      numPagesFinalThreads + numPagesToCrawl % numThreads;

  generalCrawlTasks[0].set(this->storageDirectory, &this->mustMatchPatterns,
                           &this->avoidPatterns, numPagesFirstThread,
                           pageScheduler, this->viewedUrls,
                           this->siteAttributesMap, this->lastCrawlEndTimeMap,
                           &popMutex, &storeMutex, &pushMutex, &memoryMutex);
  generalCrawlPool.addTask(&generalCrawlTasks[0]);

  for (int i = 1; i < numThreads; i++) {
    generalCrawlTasks[i].set(this->storageDirectory, &this->mustMatchPatterns,
                             &this->avoidPatterns, numPagesFinalThreads,
                             pageScheduler, this->viewedUrls,
                             this->siteAttributesMap, this->lastCrawlEndTimeMap,
                             &popMutex, &storeMutex, &pushMutex, &memoryMutex);
    generalCrawlPool.addTask(&generalCrawlTasks[i]);
  }

  pthread_mutex_destroy(&popMutex);
  pthread_mutex_destroy(&storeMutex);
  pthread_mutex_destroy(&pushMutex);
  pthread_mutex_destroy(&memoryMutex);

  delete generalCrawlTasks;
  delete pageScheduler;
}

} // namespace search_engine
