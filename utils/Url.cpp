#include "Url.hpp"
#include "Assert.hpp"
#include <CkSpider.h>
#include <iostream>
#include <string>
#include <vector>

namespace search_engine {
namespace utils {

std::string baseUrl(std::string url) {
  if (url.length() > 0) {
    std::string baseUrl = domainUrl(url);
    baseUrl = removeUrlProtocol(baseUrl);
    baseUrl = removeUrlWorldWideWeb(baseUrl);

    const std::string countryCode = ".br";
    std::size_t position = baseUrl.find(countryCode);

    utils::assertTrue(position != std::string::npos,
                      "Error(utils/baseUrl): invalid country code in " + url);

    baseUrl.erase(position, baseUrl.length());

    std::size_t numPoints = countUrlNumPoints(baseUrl);
    if (numPoints > 1) {
      const std::string point = ".";
      position = baseUrl.find(point);

      utils::assertTrue(position != std::string::npos,
                        "Error(utils/baseUrl): failed to find point in " + url);

      baseUrl.erase(baseUrl.begin(),
                    baseUrl.begin() + position + point.length());
    }

    return baseUrl;
  }
  return url;
}

std::size_t countUrlNumPoints(const std::string &url) {
  const std::string point = ".";
  std::size_t urlNumPoints = 0;
  std::size_t position = url.find(point);
  while (position != std::string::npos) {
    urlNumPoints++;
    position = url.find(point, position + point.length());
  }
  return urlNumPoints;
}

std::size_t countUrlSize(const std::string &url) {
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
  std::size_t urlSize = 0;
  position = url.find(separator, protocol.length());
  while (position != std::string::npos) {
    urlSize++;
    position = url.find(separator, position + separator.length());
  }
  return urlSize;
}

std::string domainUrl(std::string url) {
  if (url.length() > 0) {
    const std::string protocolSeparator = "://";
    std::size_t position = url.find(protocolSeparator);
    if (position != std::string::npos) {
      url.erase(url.begin(),
                url.begin() + position + protocolSeparator.length());

      const std::string linkSeparator = "//";
      position = url.find(linkSeparator);
      if (position != std::string::npos) {
        url.erase(url.begin(), url.begin() + position + linkSeparator.length());
      }
    }
  }
  return url;
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
    std::string resultUrl = domainUrl(url);

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

bool urlHasCountryCode(const std::string &url, std::string countryCode) {
  if (url.length() > 0) {
    std::string resultUrl = domainUrl(url);
    std::size_t position = resultUrl.find(countryCode);
    return position != std::string::npos;
  }
  return false;
}

bool urlHasInjection(const std::string &url) {
  if (url.length() > 0) {
    const std::string key = "{";
    std::size_t position = url.find(key);
    return position != std::string::npos;
  }
  return false;
}

} // namespace utils
} // namespace search_engine
