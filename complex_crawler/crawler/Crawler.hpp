#pragma once

namespace search_engine {

class Crawler {
public:
  Crawler();
  virtual void crawl() = 0;
};

} // namespace search_engine