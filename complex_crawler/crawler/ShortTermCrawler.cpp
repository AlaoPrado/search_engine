#include "ShortTermCrawler.hpp"
#include "scheduler/PageGroupScheduler.hpp"

namespace search_engine {

ShortTermCrawler::ShortTermCrawler(std::string storageDirectory, bool verbose)
    : Crawler(storageDirectory, verbose) {
  this->pageScheduler = new PageGroupScheduler();
}

ShortTermCrawler::~ShortTermCrawler() { delete this->pageScheduler; }

void ShortTermCrawler::crawl(std::vector<std::string> &seedUrls,
                             std::size_t numPagesToCrawl) {}

} // namespace search_engine
