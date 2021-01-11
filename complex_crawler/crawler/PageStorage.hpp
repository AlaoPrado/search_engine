#pragma once
#include <CkSpider.h>
#include <string>

namespace search_engine {

class PageStorage {
private:
  static const std::string indexName;

  PageStorage() {}

public:
  static void storePage(std::string directory, CkSpider &spider,
                        std::size_t pageId);
};

} // namespace search_engine
