#include "LongTermCrawler.hpp"
#include "../../utils/Url.hpp"
#include "Crawl.hpp"
#include "scheduler/PriorityUrlScheduler.hpp"
#include <CkSpider.h>
#include <chrono>
#include <exception>
#include <iostream>
namespace search_engine {

LongTermCrawler::LongTermCrawler(bool verbose) : Crawler(verbose) {
  this->pageScheduler = new PriorityUrlScheduler();
}

LongTermCrawler::~LongTermCrawler() { delete this->pageScheduler; }

void LongTermCrawler::pushUrlsIntoScheduler(
    CkSpider &spider, std::map<std::string, bool> &viewedUrls,
    std::size_t numPagesToCrawl) {
  std::map<std::string, bool>::iterator it;

  for (int i = 0; i < spider.get_NumUnspidered(); i++) {
    if (viewedUrls.size() >= numPagesToCrawl) {
      break;
    }

    std::string url = utils::canonicalizeUrl(spider.getUnspideredUrl(i));
    std::string urlWithouProtocol = utils::removeUrlProtocol(url);
    it = viewedUrls.find(urlWithouProtocol);
    if (it == viewedUrls.end()) {
      try {
        this->pageScheduler->push(url);
        viewedUrls[urlWithouProtocol] = true;
      } catch (std::exception &e) {
        std::cout << e.what() << std::endl;
      }
    }
  }
}

void LongTermCrawler::crawl(std::vector<std::string> &seedUrls,
                            std::size_t numPagesToCrawl) {
  auto *viewedUrls = new std::map<std::string, bool>();

  for (auto url : seedUrls) {
    std::string canonicalUrl = utils::canonicalizeUrl(url);
    this->pageScheduler->push(canonicalUrl);
    std::string urlWithouProtocol = utils::removeUrlProtocol(url);
    (*viewedUrls)[urlWithouProtocol] = true;
  }

  std::string url;
  Crawl::timePoint lastCrawlTime;
  double totalTime = 0;

  for (std::size_t i = 0; i < numPagesToCrawl; i++) {
    url = this->pageScheduler->pop();
    CkSpider spider;
    Crawl::crawlUrl(spider, url, this->mustMatchPatterns, this->avoidPatterns,
                    totalTime, lastCrawlTime, totalTime > 0);
    std::cout << spider.lastUrl() << std::endl;
    std::cout << spider.lastHtmlTitle() << std::endl;
    std::cout << spider.get_NumUnspidered() << std::endl;
    this->pushUrlsIntoScheduler(spider, *viewedUrls, numPagesToCrawl);
  }

  delete viewedUrls;
}

} // namespace search_engine
