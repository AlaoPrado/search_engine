#pragma once

#include <string>

namespace search_engine {
namespace utils {

std::string canonicalizeUrl(std::string url);

std::string removeUrlProtocol(std::string url);

} // namespace utils
} // namespace search_engine
