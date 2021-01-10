#include "LongTermCrawler.hpp"
#include "../../utils/Url.hpp"
#include "Crawl.hpp"
#include <CkSpider.h>
#include <chrono>
#include <exception>
#include <iostream>
namespace search_engine {

LongTermCrawler::LongTermCrawler(bool verbose) : Crawl(verbose) {}

void LongTermCrawler::pushUrlsIntoScheduler(
    CkSpider &spider, std::map<std::string, bool> &viewedUrls,
    std::size_t numPagesToCrawl) {
  std::map<std::string, bool>::iterator it;

  for (int i = 0; i < spider.get_NumUnspidered(); i++) {
    if (viewedUrls.size() >= numPagesToCrawl) {
      break;
    }
    
    std::string url = utils::canonicalizeUrl(spider.getUnspideredUrl(i));
    std::string urlWithouProtocol = utils::removeUrlProtocol(urlWithouProtocol);
    if (it.find(urlWithouProtocol) != viewedUrls.end()) {
      try {
        this->priorityUrlScheduler.push(url);
        viewedUrls[urlWithouProtocol] = true;
      } catch (std::exception &e) {
        // ignore exception
      }
    }
  }
}

void LongTermCrawler::crawl(std::vector<std::string> &seedUrls,
                            std::size_t numPagesToCrawl) {
  for (auto url : seedUrls) {
    this->priorityUrlScheduler.push(url);
  }

  std::string url;
  Crawl::timePoint lastCrawlTime;
  double totalTime = 0;
  auto *viewedUrls = new std::map<std::string, bool>();

  for (std::size_t i = 0; i < numPagesToCrawl; i++) {
    url = this->priorityUrlScheduler.pop();
    CkSpider spider;
    Crawl::crawlUrl(spider, url, totalTime, lastCrawlTime, totalTime == 0);
    std::cout << spider.lastUrl() << std::endl;
    std::cout << spider.lastHtmlTitle() << std::endl;
    this->pushUrlsIntoScheduler(spider, *viewedUrls, i);
  }
}

} // namespace search_engine
