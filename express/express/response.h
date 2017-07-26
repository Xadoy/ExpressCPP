#pragma once
#include <unordered_map>

namespace CExpress {
  class Response {
  public:
    Response();
    void setBodyWithFile(std::string);
    std::unordered_map<std::string, std::string> headers;
    std::string body;
    std::string status;
  };
}