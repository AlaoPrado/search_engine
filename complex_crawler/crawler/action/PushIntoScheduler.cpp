#include "PushIntoScheduler.hpp"
#include "../../../utils/Assert.hpp"
#include "../../../utils/Url.hpp"
#include "../Page.hpp"
#include <iostream>

namespace search_engine {

void PushIntoScheduler::push(PageScheduler *pageScheduler, Page &page,
                             std::map<std::string, bool> *viewedUrls,
                             pthread_mutex_t *memoryMutex) {
  utils::assertTrue(
      pageScheduler,
      "Error(PushIntoScheduler): pageScheduler has not been initialized");

  utils::assertTrue(
      viewedUrls,
      "Error(PushIntoScheduler): viewedUrls has not been initialized");

  page.setUrl(utils::standardUrl(page.getUrl()));
  std::string urlWithoutProtocol = utils::removeUrlProtocol(page.getUrl());
  std::string urlWithoutStartingPart =
      utils::removeUrlWorldWideWeb(urlWithoutProtocol);

  auto it = viewedUrls->find(urlWithoutStartingPart);
  if (it == viewedUrls->end()) {
    try {
      pageScheduler->push(page);

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
                             std::vector<std::string> &urls,
                             std::map<std::string, bool> *viewedUrls,
                             std::size_t numPagesToCrawl,
                             pthread_mutex_t *memoryMutex) {
  for (auto url : urls) {
    if (viewedUrls->size() >= numPagesToCrawl) {
      break;
    }

    Page page(url, 0);
    PushIntoScheduler::push(pageScheduler, page, viewedUrls, memoryMutex);
  }
}

void PushIntoScheduler::push(PageScheduler *pageScheduler, CkSpider &spider,
                             std::map<std::string, bool> *viewedUrls,
                             std::size_t numPagesToCrawl, int pageLevel,
                             pthread_mutex_t *memoryMutex) {
  for (int i = 0; i < spider.get_NumOutboundLinks(); i++) {
    if (viewedUrls->size() >= numPagesToCrawl) {
      break;
    }

    Page page(spider.getOutboundLink(i), 0);
    PushIntoScheduler::push(pageScheduler, page, viewedUrls, memoryMutex);
  }

  if (pageLevel == 0) {
    for (int i = 0; i < spider.get_NumUnspidered(); i++) {
      if (viewedUrls->size() >= numPagesToCrawl) {
        break;
      }

      Page page(spider.getUnspideredUrl(i), 1);
      PushIntoScheduler::push(pageScheduler, page, viewedUrls, memoryMutex);
    }
  }
}

} // namespace search_engine
