#pragma once

#include <CkSpider.h>
#include <map>
#include <string>
#include <vector>

namespace search_engine {

class Crawler {
protected:
  std::string storageDirectory;
  bool verbose;
  std::vector<std::string> mustMatchPatterns, avoidPatterns;
  std::map<std::string, bool> *viewedUrls;

public:
  Crawler(std::string storageDirectory, bool verbose = true);
  virtual ~Crawler();
  virtual void crawl(std::vector<std::string> &seedUrls,
                     std::size_t numPagesToCrawl) = 0;
};

} // namespace search_engine
