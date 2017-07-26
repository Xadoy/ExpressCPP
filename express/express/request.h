#pragma once

#include <unordered_map>

namespace CExpress {
  class Request {
  public:
    Request();
    std::unordered_map<std::string, std::string> parameters;
    std::unordered_map<std::string, std::string> queries;
    std::string body;
    std::unordered_map<std::string, std::string> headers;
  };
} // namespace CExpress