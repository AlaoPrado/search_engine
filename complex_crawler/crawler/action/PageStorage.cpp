/* Copyright 2021 Alan Prado Araújo

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
Author: alanpradoaraujo@gmail.com (Alan Prado Araújo) */
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
