#pragma once

#include <string>

namespace search_engine {
namespace utils {

std::string baseUrl(std::string url);

std::size_t countUrlSize(const std::string &url);

std::string domainUrl(std::string url);

std::string removeUrlProtocol(std::string url);

std::string removeUrlWorldWideWeb(std::string url);

std::string standardUrl(std::string url);

bool urlHasCountryCode(const std::string &url, std::string countryCode);

bool urlHasInjection(const std::string &url);

} // namespace utils
} // namespace search_engine
