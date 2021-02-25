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
#include "PageGroupScheduler.hpp"
#include "../../../utils/Assert.hpp"
#include "../../../utils/Url.hpp"
#include "PageScheduler.hpp"
#include "SimplePageScheduler.hpp"
#include <iostream>
#include <map>

namespace search_engine {

PageGroupEntry::PageGroupEntry(std::string baseUrl,
                               PageScheduler *pageScheduler)
    : baseUrl(baseUrl), pageScheduler(pageScheduler) {}

std::string PageGroupEntry::getBaseUrl() { return this->baseUrl; }

PageScheduler *PageGroupEntry::getPageScheduler() {
  return this->pageScheduler;
}

PageGroupScheduler::PageGroupScheduler() : numPages(0) {
  this->pageGroupScheduler = new std::map<std::string, PageScheduler *>();
  this->pageGroupInWork = new std::map<std::string, bool>();
  this->pageGroupQueue = new std::queue<PageGroupEntry>();
}

PageGroupScheduler::~PageGroupScheduler() {
  for (auto it = this->pageGroupScheduler->begin();
       it != this->pageGroupScheduler->end(); it++) {
    delete it->second;
  }
  delete this->pageGroupScheduler;
  delete this->pageGroupInWork;
  delete this->pageGroupQueue;
}

Page PageGroupScheduler::pop() {
  // std::cout << "PageGroupScheduler pop pageGroupQueue size: " +
  //                  std::to_string(this->pageGroupQueue->size())
  //           << std::endl;
  utils::assertTrue(!this->pageGroupQueue->empty(),
                    "Error(PageGroupScheulder/pop): blocked");

  PageScheduler *pageScheduler =
      this->pageGroupQueue->front().getPageScheduler();

  utils::assertTrue(
      pageScheduler != NULL && !pageScheduler->empty(),
      "Error(PageGroupScheulder/pop): trying to pop empty pageGroup");

  Page page = pageScheduler->pop();
  std::string baseUrl = this->pageGroupQueue->front().getBaseUrl();

  this->pageGroupQueue->pop();
  this->pageGroupInWork->operator[](baseUrl) = true;
  this->numPages--;
  return page;
}

void PageGroupScheduler::push(Page page) {
  std::string baseUrl = utils::baseUrl(page.getUrl());
  // std::cout << "PageGroupScheduler push " + baseUrl << std::endl;
  auto it = this->pageGroupScheduler->find(baseUrl);
  if (it == this->pageGroupScheduler->end()) {
    this->pageGroupScheduler->operator[](baseUrl) = new SimplePageScheduler();
    this->pageGroupInWork->operator[](baseUrl) = false;
    // std::cout << "PageGroupScheduler push " + baseUrl << std::endl;
  }
  this->pageGroupScheduler->operator[](baseUrl)->push(page);

  if (!this->pageGroupInWork->operator[](baseUrl)) {
    // std::cout << "PageGroup push " + page.getUrl() + " - " + baseUrl + ": " +
    //                  std::to_string(
    //                      this->pageGroupScheduler->operator[](baseUrl)->size())
    //           << std::endl;
    this->pageGroupQueue->push(
        PageGroupEntry(baseUrl, this->pageGroupScheduler->operator[](baseUrl)));
    this->pageGroupInWork->operator[](baseUrl) = true;
  }
  // std::cout << "PageGroupScheduler push " + baseUrl + " pageGroupQueue size:
  // " +
  //                  std::to_string(this->pageGroupQueue->size())
  //           << std::endl;
  this->numPages++;
}

std::size_t PageGroupScheduler::size() { return this->numPages; }

bool PageGroupScheduler::empty() { return this->numPages == 0; }

void PageGroupScheduler::finishWork(std::string url) {
  std::string baseUrl = utils::baseUrl(url);
  // std::cout << "PageGroupScheduler finishWork " + baseUrl << std::endl;
  auto it = this->pageGroupInWork->find(baseUrl);

  utils::assertTrue(
      it != this->pageGroupInWork->end(),
      "Error(PageGroupScheduler): the input URL does not belong to any groups");

  utils::assertTrue(it->second,
                    "Error(PageGroupScheduler): finish finished work: " + url +
                        " : " + baseUrl);

  if (it->second) {
    if (!this->pageGroupScheduler->operator[](baseUrl)->empty()) {
      this->pageGroupQueue->push(PageGroupEntry(
          baseUrl, this->pageGroupScheduler->operator[](baseUrl)));
    } else {
      this->pageGroupInWork->operator[](baseUrl) = false;
    }
  }

  // std::cout << "PageGroupScheduler finishWork pageGroupQueue size: " +
  //                  std::to_string(this->pageGroupQueue->size())
  //           << std::endl;
}

bool PageGroupScheduler::blocked() { return this->pageGroupQueue->empty(); }

} // namespace search_engine
