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
#include "PriorityPageScheduler.hpp"
#include "../../../utils/Assert.hpp"
#include "../../../utils/Url.hpp"
#include <string>
#include <vector>
namespace search_engine {

PageCompare::PageCompare() {}

bool PageCompare::operator()(PageEntry &x, PageEntry &y) const {
  return x.getUrlSize() > y.getUrlSize();
}

PriorityPageScheduler::PriorityPageScheduler() {
  this->priorityQueue =
      new std::priority_queue<PageEntry, std::vector<PageEntry>, PageCompare>();
}

PriorityPageScheduler::~PriorityPageScheduler() { delete this->priorityQueue; }

void PriorityPageScheduler::push(Page page) {
  std::size_t urlSize = utils::countUrlSize(page.getUrl());
  this->priorityQueue->push(PageEntry(page, urlSize));
}

Page PriorityPageScheduler::pop() {
  utils::assertTrue(
      !this->empty(),
      "Error(PriorityPageScheduler): pop when scheduler is empty");
  PageEntry pageEntry = this->priorityQueue->top();
  this->priorityQueue->pop();
  return pageEntry.getPage();
}

std::size_t PriorityPageScheduler::size() {
  return this->priorityQueue->size();
}

bool PriorityPageScheduler::empty(){
  return this->priorityQueue->empty();
}

} // namespace search_engine
