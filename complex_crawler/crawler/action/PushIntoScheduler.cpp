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

  std::string canonicalUrl = utils::canonicalizeUrl(url);
  std::string urlWithoutProtocol = utils::removeUrlProtocol(canonicalUrl);
  std::string urlWithoutStartingPart =
      utils::removeUrlWorldWideWeb(urlWithoutProtocol);

  auto it = viewedUrls->find(urlWithoutStartingPart);
  if (it == viewedUrls->end()) {
    try {
      pageScheduler->push(canonicalUrl);

      if (memoryMutex != NULL) {
        pthread_mutex_lock(memoryMutex);
      }

      viewedUrls->operator[](urlWithoutStartingPart) = true;

      if (memoryMutex != NULL) {
        pthread_mutex_unlock(memoryMutex);
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
  for (int i = 0; i < spider.get_NumOutboundLinks(); i++) {
    if (viewedUrls->size() >= numPagesToCrawl) {
      break;
    }

    std::string url("");
    url.append(spider.getOutboundLink(i));
    PushIntoScheduler::push(pageScheduler, url, viewedUrls, memoryMutex);
  }

  for (int i = 0; i < spider.get_NumUnspidered(); i++) {
    if (viewedUrls->size() >= numPagesToCrawl) {
      break;
    }

    std::string url("");
    url.append(spider.getUnspideredUrl(i));
    PushIntoScheduler::push(pageScheduler, url, viewedUrls, memoryMutex);
  }
}

} // namespace search_engine
