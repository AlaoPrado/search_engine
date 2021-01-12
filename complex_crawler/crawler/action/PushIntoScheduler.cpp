#include "PushIntoScheduler.hpp"
#include "../../../utils/Assert.hpp"
#include "../../../utils/Url.hpp"
#include <iostream>

namespace search_engine {

void PushIntoScheduler::push(PageScheduler *pageScheduler, std::string url,
                             std::map<std::string, bool> *viewedUrls,
                             pthread_mutex_t *memoryMutex) {
  utils::assertTrue(
      pageScheduler,
      "Error(PushIntoScheduler): pageScheduler has not been initialized");

  utils::assertTrue(
      viewedUrls,
      "Error(PushIntoScheduler): viewedUrls has not been initialized");

  std::cout << "PushIntoScheduler push inside begin" << std::endl;

  std::string canonicalUrl = utils::canonicalizeUrl(url);

  std::cout << "PushIntoScheduler" + canonicalUrl << std::endl;

  std::string urlWithouProtocol = utils::removeUrlProtocol(canonicalUrl);

  auto it = viewedUrls->find(urlWithouProtocol);
  if (it == viewedUrls->end()) {
    try {
      pageScheduler->push(canonicalUrl);
      if (memoryMutex != NULL) {
        pthread_mutex_lock(memoryMutex);
        viewedUrls->operator[](urlWithouProtocol) = true;
        pthread_mutex_unlock(memoryMutex);
      } else {
        viewedUrls->operator[](urlWithouProtocol) = true;
      }

    } catch (std::exception &e) {
      std::cout << e.what() << std::endl;
    }
  }
}

void PushIntoScheduler::push(PageScheduler *pageScheduler,
                             std::vector<std::string> urls,
                             std::map<std::string, bool> *viewedUrls,
                             std::size_t numPagesToCrawl,
                             pthread_mutex_t *memoryMutex) {
  for (auto url : urls) {
    if (viewedUrls->size() >= numPagesToCrawl) {
      break;
    }

    PushIntoScheduler::push(pageScheduler, url, viewedUrls, memoryMutex);
  }
}

void PushIntoScheduler::push(PageScheduler *pageScheduler, CkSpider &spider,
                             std::map<std::string, bool> *viewedUrls,
                             std::size_t numPagesToCrawl,
                             pthread_mutex_t *memoryMutex) {

  std::cout << "PushIntoScheduler push " +
                   std::to_string(spider.get_NumUnspidered())
            << std::endl;
  for (int i = 0; i < spider.get_NumUnspidered(); i++) {
    std::cout << "PushIntoScheduler push loop begin" << std::endl;

    if (viewedUrls->size() >= numPagesToCrawl) {
      break;
    }

    std::cout << "PushIntoScheduler break pass" << std::endl;

    std::string url("");
    url.append(spider.getUnspideredUrl(i));

    std::cout << url << std::endl;
    // std::cout << spider.getUnspideredUrl(i) << std::endl;

    PushIntoScheduler::push(pageScheduler, url, viewedUrls, memoryMutex);
  }
}

} // namespace search_engine
