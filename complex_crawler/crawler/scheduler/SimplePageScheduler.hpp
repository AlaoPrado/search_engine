#pragma once

#include "PriorityPageScheduler.hpp"
#include "UrlEntry.hpp"
#include <queue>
#include <string>

namespace search_engine {

class SimplePageScheduler : public PriorityPageScheduler {
private:
  std::queue<UrlEntry> *urlQueue;

public:
  SimplePageScheduler();
  virtual ~SimplePageScheduler() override;
  virtual std::string pop() override;
  virtual void push(std::string url) override;
  virtual std::size_t size() override;
  virtual bool empty() override;
};

} // namespace search_engine
