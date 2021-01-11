#include "PageStorage.hpp"
#include "../../utils/Assert.hpp"
#include <CkSpider.h>
#include <fstream>
#include <string>

namespace search_engine {

const std::string PageStorage::indexName("index.txt");

void PageStorage::storePage(std::string directory, CkSpider &spider,
                            std::size_t pageId) {
  std::ofstream indexFile(directory + indexName,
                          std::ofstream::out | std::ofstream::app);

  utils::assertTrue(indexFile.is_open(),
                    "Error(PageStorage): failed to open index file");

  indexFile << spider.lastUrl() << " " << pageId << std::endl;
  indexFile.close();
}

} // namespace search_engine
