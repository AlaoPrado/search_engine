#include "Crawler.hpp"
#include "../../utils/Assert.hpp"
#include "../../utils/Url.hpp"
#include <CkSpider.h>
#include <exception>
#include <iostream>
#include <map>
#include <string>

namespace search_engine {

Crawler::Crawler(std::string storageDirectory, bool verbose)
    : storageDirectory(storageDirectory), verbose(verbose) {
  this->mustMatchPatterns.push_back("*.br*");
  this->avoidPatterns.push_back("*.pdf");
  this->viewedUrls = new std::map<std::string, bool>();
}

Crawler::~Crawler() { delete this->viewedUrls; }

void Crawler::pushUrlIntoScheduler(std::string url) {
  utils::assertTrue(this->pageScheduler,
                    "Error(Crawler): pageScheduler has not been initialized");

  std::string canonicalUrl = utils::canonicalizeUrl(url);
  std::string urlWithouProtocol = utils::removeUrlProtocol(canonicalUrl);
  std::map<std::string, bool>::iterator it =
      this->viewedUrls->find(urlWithouProtocol);

  if (it == this->viewedUrls->end()) {
    try {
      this->pageScheduler->push(canonicalUrl);
      this->viewedUrls->operator[](urlWithouProtocol) = true;
    } catch (std::exception &e) {
      std::cout << e.what() << std::endl;
    }
  }
}

void Crawler::pushUrlsIntoScheduler(std::vector<std::string> urls,
                                    std::size_t numPagesToCrawl) {
  for (auto url : urls) {
    if (this->viewedUrls->size() >= numPagesToCrawl) {
      break;
    }

    this->pushUrlIntoScheduler(url);
  }
}

void Crawler::pushUrlsIntoScheduler(CkSpider &spider,
                                    std::size_t numPagesToCrawl) {
  for (int i = 0; i < spider.get_NumUnspidered(); i++) {
    if (this->viewedUrls->size() >= numPagesToCrawl) {
      break;
    }

    this->pushUrlIntoScheduler(spider.getUnspideredUrl(i));
  }
}

} // namespace search_engine
