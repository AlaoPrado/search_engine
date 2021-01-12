#include "PageGroupScheduler.hpp"
#include "../../../utils/Assert.hpp"
#include "../../../utils/Url.hpp"
#include "PageScheduler.hpp"
#include "SimplePageScheduler.hpp"
#include <map>

namespace search_engine {

std::map<std::string, PageScheduler *> *pageGroupScheduler;

PageGroupScheduler::PageGroupScheduler() : numPages(0) {
  this->pageGroupScheduler = new std::map<std::string, PageScheduler *>();
  this->pageGroupInWork = new std::map<std::string, bool>();
}

PageGroupScheduler::~PageGroupScheduler() {
  for (auto it = this->pageGroupScheduler->begin();
       it != this->pageGroupScheduler->end(); it++) {
    delete it->second;
  }
  delete this->pageGroupScheduler;
  delete this->pageGroupInWork;
}

std::string PageGroupScheduler::pop() {
  for (auto it = this->pageGroupInWork->begin();
       it != this->pageGroupInWork->end(); it++) {
    if (!it->second &&
        this->pageGroupScheduler->operator[](it->first)->size() > 0) {
      this->numPages--;
      it->second = true;
      return this->pageGroupScheduler->operator[](it->first)->pop();
    }
  }

  utils::assertTrue(false, "Error(PageGroupScheduler): no urls to pop");
  return nullptr;
}

void PageGroupScheduler::push(std::string url) {
  std::string baseUrl = utils::baseUrl(url);
  auto it = this->pageGroupScheduler->find(baseUrl);
  if (it == this->pageGroupScheduler->end()) {
    this->pageGroupScheduler->operator[](baseUrl) = new SimplePageScheduler();
    this->pageGroupInWork->operator[](baseUrl) = false;
  }
  this->pageGroupScheduler->operator[](baseUrl)->push(url);
  this->numPages++;
}

std::size_t PageGroupScheduler::size() { return this->numPages; }

void PageGroupScheduler::finishWork(std::string url) {
  std::string baseUrl = utils::baseUrl(url);
  auto it = this->pageGroupInWork->find(baseUrl);

  utils::assertTrue(
      it != this->pageGroupInWork->end(),
      "Error(PageGroupScheduler): the input URL does not belong to any groups");

  it->second = false;
}

} // namespace search_engine
