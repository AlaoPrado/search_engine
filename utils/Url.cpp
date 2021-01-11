#include "Url.hpp"
#include "Assert.hpp"
#include <string>
#include <vector>

namespace search_engine {
namespace utils {

std::string canonicalizeUrl(std::string url) {
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
                    "Error(PriorityUrlScheduler): invalid url protocol");

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

} // namespace utils
} // namespace search_engine
