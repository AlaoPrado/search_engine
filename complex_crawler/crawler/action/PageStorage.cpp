#include "PageStorage.hpp"
#include "../../../utils/Assert.hpp"
#include <CkSpider.h>
#include <fstream>
#include <string>

namespace search_engine {

const std::string PageStorage::indexName("index.txt");

void PageStorage::storePage(std::string directory, CkSpider &spider,
                            std::size_t pageId, pthread_mutex_t *mutex) {
  std::string html(spider.lastHtml());

  utils::assertTrue(html.size() > 0, "Error(PageStorage): page is empty");

  const std::string fileExtension = ".html";
  std::ofstream htmlFile(directory + std::to_string(pageId) + fileExtension,
                         std::ofstream::out | std::ofstream::trunc);

  htmlFile << html;
  htmlFile.close();

  if (mutex != NULL) {
    pthread_mutex_lock(mutex);
  }

  std::ofstream indexFile(directory + indexName,
                          std::ofstream::out | std::ofstream::app);

  utils::assertTrue(indexFile.is_open(),
                    "Error(PageStorage): failed to open index file");

  indexFile << spider.lastUrl() << " " << pageId << std::endl;
  indexFile.close();

  if (mutex != NULL) {
    pthread_mutex_unlock(mutex);
  }
}

} // namespace search_engine
