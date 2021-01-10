#pragma once

#include <string>
#include <vector>

namespace search_engine {

class Crawler {
protected:
  ;
  bool verbose;
  std::vector<std::string> mustMatchPatterns, avoidPatterns;

public:
  Crawler(bool verbose = true);
  virtual void crawl(std::vector<std::string> &seedUrls,
                     std::size_t numPagesToCrawl) = 0;
};

} // namespace search_engine
