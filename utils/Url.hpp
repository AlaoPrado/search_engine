#pragma once

#include <string>

namespace search_engine {
namespace utils {

std::string baseUrl(std::string url);

std::string canonicalizeUrl(std::string url);

std::size_t countUrlSize(std::string url);

std::string removeUrlProtocol(std::string url);

std::string removeUrlWorldWideWeb(std::string url);

} // namespace utils
} // namespace search_engine
