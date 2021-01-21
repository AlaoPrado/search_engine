#include "PushIntoScheduler.hpp"
#include "../../../utils/Assert.hpp"
#include "../../../utils/Url.hpp"
#include "../Page.hpp"
#include <iostream>

namespace search_engine {

bool PushIntoScheduler::pushedSufficient(
    std::map<std::string, bool> &viewedUrls, std::size_t numPagesToCrawl) {
  constexpr double extraMargin = 1.2;
  return viewedUrls.size() >= (std::size_t)(numPagesToCrawl * extraMargin);
}

void PushIntoScheduler::push(PageScheduler &pageScheduler, Page &page,
                             std::map<std::string, bool> &viewedUrls,
                             int &numPagesPushed, pthread_mutex_t *memoryMutex,
                             pthread_mutex_t *pushMutex) {
  std::string url = utils::standardUrl(page.getUrl());

  if (utils::urlHasCountryCode(url, ".br") && !utils::urlHasInjection(url) &&
      !utils::urlHasExternalLink(url) && !utils::urlHasComposeDomain(url)) {
    page.setUrl(url);
    std::string urlWithoutProtocol = utils::removeUrlProtocol(page.getUrl());
    std::string urlWithoutStartingPart =
        utils::removeUrlWorldWideWeb(urlWithoutProtocol);
    std::map<std::string, bool>::iterator it;

    if (pushMutex != NULL) {
      pthread_mutex_lock(pushMutex);
      it = viewedUrls.find(urlWithoutStartingPart);
      pthread_mutex_unlock(pushMutex);
    } else {
      it = viewedUrls.find(urlWithoutStartingPart);
    }

    if (it == viewedUrls.end()) {
      try {
        pageScheduler.push(page);

        if (memoryMutex != NULL) {
          pthread_mutex_lock(memoryMutex);
        }

        if (pushMutex != NULL) {
          pthread_mutex_lock(pushMutex);
          viewedUrls.operator[](urlWithoutStartingPart) = true;
          pthread_mutex_unlock(pushMutex);
        } else {
          viewedUrls.operator[](urlWithoutStartingPart) = true;
        }

        if (memoryMutex != NULL) {
          pthread_mutex_unlock(memoryMutex);
        }

        numPagesPushed++;
      } catch (std::exception &e) {
        std::cout << "Error while Pushing " << e.what() << std::endl;
      }
    }
  }
}

void PushIntoScheduler::push(PageScheduler &pageScheduler,
                             std::vector<std::string> &urls,
                             std::map<std::string, bool> &viewedUrls,
                             std::size_t numPagesToCrawl, int &numPagesPushed,
                             pthread_mutex_t *memoryMutex,
                             pthread_mutex_t *pushMutex) {
  bool end = false;

  numPagesPushed = 0;
  for (auto url : urls) {
    if (pushMutex != NULL) {
      pthread_mutex_lock(pushMutex);
      end = PushIntoScheduler::pushedSufficient(viewedUrls, numPagesToCrawl);
      pthread_mutex_unlock(pushMutex);
    } else {
      end = PushIntoScheduler::pushedSufficient(viewedUrls, numPagesToCrawl);
    }

    if (end) {
      break;
    }

    Page page(url, 0);
    PushIntoScheduler::push(pageScheduler, page, viewedUrls, numPagesPushed,
                            memoryMutex, pushMutex);
  }
}

void PushIntoScheduler::push(PageScheduler &pageScheduler, CkSpider &spider,
                             std::map<std::string, bool> &viewedUrls,
                             std::size_t numPagesToCrawl, int pageLevel,
                             int &numPagesPushed, pthread_mutex_t *memoryMutex,
                             pthread_mutex_t *pushMutex) {
  bool end = false;

  numPagesPushed = 0;
  for (int i = 0; i < spider.get_NumOutboundLinks(); i++) {
    if (pushMutex != NULL) {
      pthread_mutex_lock(pushMutex);
      end = PushIntoScheduler::pushedSufficient(viewedUrls, numPagesToCrawl);
      pthread_mutex_unlock(pushMutex);
    } else {
      end = PushIntoScheduler::pushedSufficient(viewedUrls, numPagesToCrawl);
    }

    if (end) {
      break;
    }

    Page page(spider.getOutboundLink(i), 0);
    PushIntoScheduler::push(pageScheduler, page, viewedUrls, numPagesPushed,
                            memoryMutex, pushMutex);
  }

  if (pageLevel == 0) {
    for (int i = 0; i < spider.get_NumUnspidered(); i++) {
      if (pushMutex != NULL) {
        pthread_mutex_lock(pushMutex);
        end = PushIntoScheduler::pushedSufficient(viewedUrls, numPagesToCrawl);
        pthread_mutex_unlock(pushMutex);
      } else {
        end = PushIntoScheduler::pushedSufficient(viewedUrls, numPagesToCrawl);
      }

      if (end) {
        break;
      }

      Page page(spider.getUnspideredUrl(i), 1);
      PushIntoScheduler::push(pageScheduler, page, viewedUrls, numPagesPushed,
                              memoryMutex, pushMutex);
    }
  }
}

} // namespace search_engine
