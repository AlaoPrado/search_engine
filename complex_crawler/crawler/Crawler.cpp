#include "Crawler.hpp"

namespace search_engine {

Crawler::Crawler(bool verbose) : verbose(verbose) {
  this->mustMatchPatterns.push_back("*.br*");
  this->avoidPatterns.push_back("*.pdf");
}

} // namespace search_engine
