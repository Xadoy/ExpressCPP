#pragma once

#include <string>
#include <sstream>
#include <vector>

namespace CExpress {
  enum RequestType { GET, HEAD, POST, PUT, DELET, CONNECT, OPTIONS, TRACE, PATCH };

  class Route {
  public:
    /* Create a Route with the specified request type and path. */
    Route(RequestType type, const std::string & path);

    /* Get the path of this Route. */
    std::vector<std::string> getPathTokens() const;

    /* Get the request type of this Route. */
    RequestType getRequestType() const;

    /* True if source and target routes have the same path. */
    bool matches(const Route& target) const;

  private:
    RequestType requestType;

    std::vector<std::string> pathTokens;

    void splitString(const std::string &s, const char delim, std::vector<std::string>& elems) const;
  };
}

