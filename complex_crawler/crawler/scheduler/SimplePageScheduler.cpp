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
#include "SimplePageScheduler.hpp"
#include "../../../utils/Assert.hpp"
#include "../../../utils/Url.hpp"
#include <queue>
#include <string>

namespace search_engine {

SimplePageScheduler::SimplePageScheduler() {
  this->urlQueue = new std::queue<PageEntry>();
}

SimplePageScheduler::~SimplePageScheduler() { delete this->urlQueue; }

Page SimplePageScheduler::pop() {
  utils::assertTrue(!this->empty(),
                    "Error(SimplePageScheduler): pop when scheduler is empty");
  PageEntry pageEntry = this->urlQueue->front();
  this->urlQueue->pop();
  return pageEntry.getPage();
}

void SimplePageScheduler::push(Page page) {
  std::size_t urlSize = utils::countUrlSize(page.getUrl());
  this->urlQueue->push(PageEntry(page, urlSize));
}

std::size_t SimplePageScheduler::size() { return this->urlQueue->size(); }

bool SimplePageScheduler::empty() { return this->urlQueue->empty(); }

} // namespace search_engine
