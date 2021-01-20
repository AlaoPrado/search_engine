#include "GeneralShorTermCrawler.hpp"
#include "../../threadpool/ThreadPool.hpp"

namespace search_engine {

GeneralShorTermCrawler::GeneralShorTermCrawler(std::string storageDirectory,
                                               bool verbose, int numThreads)
    : Crawler(storageDirectory, verbose), numThreads(numThreads) {}

GeneralShorTermCrawler::~GeneralShorTermCrawler() {}

void GeneralShorTermCrawler::crawl(std::vector<std::string> &seedUrls,
                                   std::size_t numPagesToCrawl) {
  ThreadPool generalCrawlPool(numThreads);
}

} // namespace search_engine
