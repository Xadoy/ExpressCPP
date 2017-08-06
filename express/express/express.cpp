#include <iostream>
#include <stdio.h>
#include <algorithm>

#include "express.h"
#include "helper.h"
#include "utility.h"

void CExpress::Express::use(
    const std::function<void(CExpress::Request &, CExpress::Response &)>
        &middleware_handler) {
  // check if the function is callable, if not then return immediatly.
  if (!middleware_handler)
    throw
        "Function warpper without target is not allowed for the use function";

  middlewares_.push_back(middleware_handler);
}

void CExpress::Express::route(const std::string &method, const std::string & route,
  const std::function<void(CExpress::Request &, CExpress::Response &)> & route_handler) {
  CExpress::RequestType request_type = getRequestTypeFromString(method);

  Route r(request_type, route);

  if (!route_handler) {
    throw "Function warpper without target is not allowed for the route function";
  }

  controllers_.push_back({r, route_handler});
}

void CExpress::Express::onerror(
    const std::string &error_code,
    const std::function<void(CExpress::Request &, CExpress::Response &)>
        &error_handler) {
  if (!error_handler) {
    throw "Function warpper without target is not allowed for the onerror function";
  }

  error_handlers_.insert({error_code, error_handler});
}

CExpress::Express::Express() {}

void CExpress::Express::start(int port) {
  server_ = Server();
  server_.setPort(port);
  server_.process([=](std::string incoming_str) {
    std::string response_str;
    
    // create a request and a response
    Request request;
    Response response;

    try {
      // process incoming str
      std::string request_line, request_headers, path, method;
      
      request_line = incoming_str.substr(0, incoming_str.find("\r\n"));
      request_headers = incoming_str.substr(incoming_str.find("\r\n") + 2, incoming_str.find("\r\n\r\n") - incoming_str.find("\r\n") - 2);

      size_t first_space_pos = incoming_str.find(' ');
      size_t second_space_pos = incoming_str.find(' ', first_space_pos + 1);
      method = incoming_str.substr(0, first_space_pos);
      path = incoming_str.substr(first_space_pos + 1, second_space_pos - first_space_pos - 1);

      // set request headers
      setRequestHeaders(request, request_headers);

      // set request queries
      size_t query_mark_pos = path.find('?');
      if (query_mark_pos != std::string::npos) {
        setRequestQueryParameters(request, path.substr(query_mark_pos + 1));
      }

      // find controller
      int route_index = -1;
      RequestType request_type = getRequestTypeFromString(method);
      Route route(request_type, path);
      for (size_t i = 0; i < controllers_.size(); i++) {
        const auto &registered_route = controllers_[i].first;
        if (registered_route.matches(route)) {
          setRquestParameters(request, registered_route.getPathTokens(), route.getPathTokens());
          route_index = i;
          break;
        }
      }

      // go through middleware
      for (const auto &middleware_handler : middlewares_) {
        middleware_handler(request, response);
      }

      // call the controller
      if (route_index != -1) {
        response.status = STATUS_CODE_200;
        controllers_[route_index].second(request, response);
      } else {
        // unable to find the controller

        // check if request path is /
        // if so, try find /index.htm or /index.html
        if (path == "/") {
          if (response.setBodyWithFile("index.htm") || response.setBodyWithFile("index.html")) {
            response.status = STATUS_CODE_200;
          }
          else {
            // 404
            handle404(request, response);
          }
        }
        else {
          if (response.setBodyWithFile(path.substr(1))) {
            response.status = STATUS_CODE_200;
          }
          else {
            // 404
            handle404(request, response);
          }
        }
      }
    }
    catch (const char * err) {
      handle500(request, response, err);
    }
    catch (...) {
      handle500(request, response);
    } 

    response_str = response.generateResponseString();
    return response_str;
  });
}

void CExpress::Express::handle500(Request & request, Response & response, const char * errorMsg) const {
  response.status = STATUS_CODE_500;

  if (error_handlers_.find(STATUS_CODE_500) == error_handlers_.end()) {
    // default 500
    response.headers["Content-Type"] = "text/html; charset=utf-8";
    char responseBody[1024];
    snprintf(responseBody, sizeof(responseBody), DEFAULT_500_PAGE.c_str(), errorMsg);
    response.body = std::string(responseBody);
    return;
  }

  error_handlers_.at(STATUS_CODE_500)(request, response);
}

void CExpress::Express::handle404(Request & request,  Response & response) const {
  response.status = STATUS_CODE_404;

  if (error_handlers_.find(STATUS_CODE_404) == error_handlers_.end()) {
    // default 404
    response.headers["Content-Type"] = "text/html; charset=utf-8";
    response.body = DEFAULT_404_PAGE;
    return;
  }

  error_handlers_.at(STATUS_CODE_404)(request, response);
}

void CExpress::Express::setRquestParameters(Request &request, const std::vector<std::string> &key_path_tokens, const std::vector<std::string> &value_path_tokens) {
  const char ATTRI_PLACE_HOLDER = ':';
  for (size_t i = 0; i < key_path_tokens.size(); i++) {
    if (key_path_tokens[i][0] == ATTRI_PLACE_HOLDER) {
      request.parameters[key_path_tokens[i].substr(1)] = value_path_tokens[i];
    }
  }
}

// set queries for Request
void CExpress::Express::setRequestQueryParameters(Request &request, const std::string &attributes) {
  std::vector<std::string> queries_str = std::vector<std::string>();
  splitString(attributes, '&', queries_str);
  for (const auto &query_str : queries_str) {
    size_t equal_sign_pos = query_str.find('=');
    request.queries[query_str.substr(0, equal_sign_pos)] =
        query_str.substr(equal_sign_pos + 1);
  }
}

// set headers for Request
void CExpress::Express::setRequestHeaders(Request &request, std::string &request_header) {
  std::istringstream ss(request_header);
  std::string header;
  while (std::getline(ss, header)) {
    size_t pos = header.find(':');
    if (pos == header.npos) {
      throw "invalid header";
    }
    request.headers[header.substr(0, pos)] = header.substr(pos + 2);
  }
}

const std::string CExpress::Express::DEFAULT_404_PAGE = u8"<html><body> <h1> 404 Not Found 😓 </h1> <hr/> <small>ExpressCPP | <a href=\"https://github.com/AlephTeam/ExpressCPP\" target=\"_blank\">Github</a></small></body></html>";
const std::string CExpress::Express::DEFAULT_500_PAGE = u8"<html><body> <h1> 500 Server Internal Error 😔 </h1> <br /> %s <hr/> <small>ExpressCPP | <a href=\"https://github.com/AlephTeam/ExpressCPP\" target=\"_blank\">Github</a></small></body></html>";
const std::string CExpress::Express::STATUS_CODE_200 = "200";
const std::string CExpress::Express::STATUS_CODE_404 = "404";
const std::string CExpress::Express::STATUS_CODE_500 = "500";
