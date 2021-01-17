#include "Page.hpp"

namespace search_engine {

Page::Page(std::string url, int level) : url(url), level(level) {}

std::string Page::getUrl() { return this->url; }

void Page::setUrl(std::string url) { this->url = url; }

int Page::getLevel() { return this->level; }

} // namespace search_engine
