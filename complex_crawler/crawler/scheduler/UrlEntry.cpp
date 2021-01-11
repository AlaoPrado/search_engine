#include "UrlEntry.hpp"

namespace search_engine {

UrlEntry::UrlEntry(std::string url, int size)
    : url(url), size(size) {}

std::string UrlEntry::getUrl(){
  return this->url;
}

int UrlEntry::getSize(){
  return this->size;
}

} // namespace search_engine
