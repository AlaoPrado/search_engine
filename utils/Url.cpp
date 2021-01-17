#include "Url.hpp"
#include "Assert.hpp"
#include <CkSpider.h>
#include <string>
#include <vector>

namespace search_engine {
namespace utils {

std::string baseUrl(std::string url) {
  if (url.length() > 0) {
    CkSpider spider;
    std::string baseUrl(spider.getBaseDomain(url.c_str()));

    baseUrl = removeUrlProtocol(baseUrl);
    baseUrl = removeUrlWorldWideWeb(baseUrl);

    const std::string countryCode = ".br";
    std::size_t position = baseUrl.find(countryCode);

    utils::assertTrue(position != std::string::npos,
                      "Error(utils/baseUrl): invalid url country code");

    baseUrl.erase(position, baseUrl.length());
    return baseUrl;
  }
  return url;
}

std::size_t countUrlSize(std::string url) {
  const std::vector<std::string> validProtocols = {"http://", "https://"};

  std::size_t position = std::string::npos;
  std::string protocol;
  for (auto currentProtocol : validProtocols) {
    position = url.find(currentProtocol);
    if (position == 0) {
      protocol = currentProtocol;
      break;
    }
  }
  utils::assertTrue(position == 0,
                    "Error(utils/countUrlSize): invalid url protocol");

  const std::string separator = "/";
  std::size_t urltSize = 0;
  position = url.find(separator, protocol.length());
  while (position != std::string::npos) {
    urltSize++;
    position = url.find(separator, position + separator.length());
  }
  return urltSize;
}

std::string removeUrlProtocol(std::string url) {
  const std::string protocolSeparator = "://";
  std::size_t position = url.find(protocolSeparator);
  if (position != std::string::npos) {
    url.erase(url.begin(), url.begin() + position + protocolSeparator.length());
  }
  return url;
}

std::string removeUrlWorldWideWeb(std::string url) {
  const std::string worldWideWeb = "www.";
  std::size_t position = url.find(worldWideWeb);
  if (position != std::string::npos) {
    url.erase(position, worldWideWeb.length());
  }
  return url;
}

std::string standardUrl(std::string url) {
  if (url.length() > 0) {
    if (url[url.length() - 1] == '/') {
      url.erase(url.end() - 1);
    }

    const std::string protocolSeparator = "://";
    std::size_t position = url.find(protocolSeparator);
    if (position == std::string::npos) {
      url = "https://" + url;
    }
  }
  return url;
}

} // namespace utils
} // namespace search_engine
