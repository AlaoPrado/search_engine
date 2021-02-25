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
#include "StorePageTask.hpp"
#include "../../../threadpool/CounterFlag.hpp"
#include "../action/PageStorage.hpp"
#include <CkSpider.h>
#include <iostream>
#include <string>

namespace search_engine {

StorePageTask::StorePageTask(CounterFlag *counterFlag, std::string directory,
                             CkSpider *spider, std::size_t pageId,
                             bool *success)
    : counterFlag(counterFlag), directory(directory), spider(spider),
      pageId(pageId), success(success) {}

StorePageTask::~StorePageTask() {}

void StorePageTask::run() {
  std::string url(spider->lastUrl());
  try {
    std::cout << "Store url: " + url << std::endl;
    PageStorage::storePage(directory, *spider, pageId);
    *success = true;
  } catch (std::exception &e) {
    std::cout << "Error while storing page " + url << std::endl;
    std::cout << e.what() << std::endl;
    *success = false;
  }
  std::cout << "Store finished: " + url << std::endl;
  counterFlag->signal();
}

} // namespace search_engine
