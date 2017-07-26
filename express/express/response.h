#pragma once
#include <unordered_map>

namespace CExpress {
  class Response {
  public:
    std::unordered_map<std::string, std::string> Header;
    std::string Body;
    std::string Status;

    Response();
    void setBodyWithFile(std::string);
  };
}