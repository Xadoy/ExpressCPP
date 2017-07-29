#include <stdio.h>
#include <iostream>

#include "express.h"
#include "helper.h"

void CExpress::Express::use(const std::function<void(CExpress::Request&, CExpress::Response&)>& middleware_handler)
{
  // check if the function is callable, if not then return immediatly.
  if (!middleware_handler)
    throw ("Function warpper without target is not allowed for the use function");

  middleWares_.push_back(middleware_handler);
}

void CExpress::Express::route(const std::string& method, const std::string& route, const std::function<void(CExpress::Request&, CExpress::Response&)>& route_handler)
{
  CExpress::RequestType request_type = getRequestTypeFromString(method);

  Route r(request_type, route);

  if (!route_handler) {
    throw "Function warpper without target is not allowed for the route function";
  }

  controllers_.push_back({ r, route_handler });
}

void CExpress::Express::onerror(const std::string& error_code, const std::function<void(CExpress::Request&, CExpress::Response&)>& error_handler)
{
  if (!error_handler) {
    throw "Function warpper without target is not allowed for the onerror function";
  }

  errorHandlers_.insert({ error_code, error_handler });
}

CExpress::Express::Express()
{
}

void CExpress::Express::start(int port)
{
  server_ = Server();
  server_.setPort(port);
  server_.process([=](std::string incoming_str) {
    // process incoming str

    // TODO:
    // create a request and a response
    // go through middleware
    // find controller
    // call the controller
    // fallback try find the file
    // otherwise, find error handlers
    // otherwise, return default error page
    std::string response_str = "HTTP/1.1 200 OK\n\
Content-Type: text/html; charset=utf-8\n\
Content-Length: 2\n\
\n\
hi";
    return response_str;
  });
}

const std::string CExpress::Express::DEFAULT_404_PAGE = "<html><body> <h1> 404 Not Found 😓 </h1> <hr/> <small>ExpressCPP | <a href=\"https://github.com/AlephTeam/ExpressCPP\" target=\"_blank\">Github</a></small></body></html>";
const std::string CExpress::Express::DEFAULT_500_PAGE = "<html><body> <h1> 500 Server Internal Error 😔 </h1> <hr/> <small>ExpressCPP | <a href=\"https://github.com/AlephTeam/ExpressCPP\" target=\"_blank\">Github</a></small></body></html>";