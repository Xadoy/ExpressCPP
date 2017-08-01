#pragma once

#include <unordered_map>
#include <string>

namespace CExpress {
  class Response {
  public:
    Response();
    /* try to fill the body with the content of the file,
    return bool indicating that the file exists*/
    bool setBodyWithFile(const std::string&);
    std::unordered_map<std::string, std::string> headers;
    std::string body;
    std::string status;

    std::string generateResponseString();

  private:
    static const std::string HTTP_VERSION;
    static const std::string REASON_PHRASE_200;
    static const std::string REASON_PHRASE_404;
    static const std::string REASON_PHRASE_500;

    static const std::unordered_map<std::string, std::string> MIME_TYPES;
  };
}