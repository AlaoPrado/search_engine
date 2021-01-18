#pragma once

#include "PageScheduler.hpp"
#include <map>
#include <queue>

namespace search_engine {

class PageGroupEntry {
private:
  std::string baseUrl;
  PageScheduler *pageScheduler;

public:
  PageGroupEntry(std::string baseUrl, PageScheduler *pageScheduler);
  std::string getBaseUrl();
  PageScheduler *getPageScheduler();
};

class PageGroupScheduler : public PageScheduler {
private:
  std::size_t numPages;
  std::map<std::string, PageScheduler *> *pageGroupScheduler;
  std::map<std::string, bool> *pageGroupInWork;
  std::queue<PageGroupEntry> *pageGroupQueue;

public:
  PageGroupScheduler();
  ~PageGroupScheduler();
  virtual Page pop() override;
  virtual void push(Page page) override;
  virtual std::size_t size() override;
  virtual bool empty() override;
  virtual void finishWork(std::string url);
  virtual bool blocked();
};

} // namespace search_engine
