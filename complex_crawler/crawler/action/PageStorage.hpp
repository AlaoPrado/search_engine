#pragma once
#include <CkSpider.h>
#include <pthread.h>
#include <string>

namespace search_engine {

class PageStorage {
private:
  static const std::string indexName;

  PageStorage() {}

public:
  static void storePage(std::string directory, CkSpider &spider,
                        std::size_t pageId,
                        pthread_mutex_t *mutex = NULL);
};

} // namespace search_engine
