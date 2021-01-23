#include "Document.hpp"

namespace search_engine {

Document::Document(std::string directory, std::string url)
    : directory(directory), url(url) {}

std::string Document::getDirectory() { return this->directory; }

std::string Document::getUrl() { return this->url; }

} // namespace search_engine
