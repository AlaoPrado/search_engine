/* Copyright 2021 Alan Prado Araújo

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
Author: alanpradoaraujo@gmail.com (Alan Prado Araújo) */
#include "Url.hpp"
#include "Assert.hpp"
#include <iostream>
#include <string>
#include <vector>

namespace search_engine {
namespace utils {

std::string baseUrl(std::string url) {
  if (url.length() > 0) {
    std::string resultString = removeUrlProtocol(url);
    resultString = removeUrlWorldWideWeb(resultString);

    const std::string countryCode = ".br";
    std::size_t position = resultString.find(countryCode);

    utils::assertTrue(position != std::string::npos,
                      "Error(utils/baseUrl): invalid country code in " + url);

    resultString.erase(position, resultString.length());
    return resultString;
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
    url = removeUrlProtocol(url);

    const std::string linkSeparator = "//";
    std::size_t position = url.find(linkSeparator);
    if (position != std::string::npos) {
      url.erase(url.begin(), url.begin() + position + linkSeparator.length());
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

bool urlHasComposeDomain(const std::string &url) {
  std::string tempUrl = baseUrl(url);
  const std::string separator = "/";
  std::size_t position = tempUrl.find(separator);

  return position != std::string::npos || countUrlNumPoints(tempUrl) > 2;
}

bool urlHasCountryCode(const std::string &url, std::string countryCode) {
  if (url.length() > 0) {
    std::size_t position = url.find(countryCode);

    if (position == std::string::npos) {
      return false;
    }

    position += countryCode.length();

    if (position == url.length()) {
      return true;
    }

    return url[position] == '/';
  }
  return false;
}

bool urlHasExternalLink(const std::string &url) {
  if (url.length() > 0) {
    std::string tempUrl = removeUrlProtocol(url);

    const std::string linkSeparator = "//";
    std::size_t position = tempUrl.find(linkSeparator);
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
