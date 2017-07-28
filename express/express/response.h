#pragma once

#include <unordered_map>
#include <string>

namespace CExpress {
  class Response {
  public:
    Response();
    void setBodyWithFile(const std::string&);
    std::unordered_map<std::string, std::string> headers;
    std::string body;
    std::string status;
  };
}