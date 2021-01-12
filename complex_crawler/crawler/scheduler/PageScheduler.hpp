#pragma once

#include <string>

namespace search_engine {

class PageScheduler {
public:
  virtual ~PageScheduler(){};
  virtual std::string pop() = 0;
  virtual void push(std::string url) = 0;
  virtual std::size_t size() = 0;
  virtual bool empty() = 0;
};

} // namespace search_engine
