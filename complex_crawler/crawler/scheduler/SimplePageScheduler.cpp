#include "SimplePageScheduler.hpp"
#include "../../../utils/Assert.hpp"
#include "../../../utils/Url.hpp"
#include <queue>
#include <string>

namespace search_engine {

SimplePageScheduler::SimplePageScheduler() {
  this->urlQueue = new std::queue<UrlEntry>();
}

SimplePageScheduler::~SimplePageScheduler() { delete this->urlQueue; }

std::string SimplePageScheduler::pop() {
  utils::assertTrue(this->size() > 0,
                    "Error(SimplePageScheduler): pop when scheduler is empty");
  UrlEntry urlEntry = this->urlQueue->front();
  this->urlQueue->pop();
  return urlEntry.getUrl();
}

void SimplePageScheduler::push(std::string url) {
  std::size_t urlSize = utils::countUrlSize(url);
  this->urlQueue->push(UrlEntry(url, urlSize));
}

std::size_t SimplePageScheduler::size() { return this->urlQueue->size(); }

bool SimplePageScheduler::empty() { return this->urlQueue->empty(); }

} // namespace search_engine
