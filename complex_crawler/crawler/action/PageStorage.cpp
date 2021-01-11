#include "PageStorage.hpp"
#include "../../../utils/Assert.hpp"
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

  const std::string fileExtension = ".html";
  std::ofstream htmlFile(directory + std::to_string(pageId) + fileExtension,
                         std::ofstream::out | std::ofstream::trunc);

  htmlFile << spider.lastHtml() << std::endl;
  htmlFile.close();
}

} // namespace search_engine
