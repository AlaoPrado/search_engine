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
