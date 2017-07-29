#pragma once

#include <string>

#include "express.h"

// try go parse a string into one of request type
// if not able to, throw an error
CExpress::RequestType getRequestTypeFromString(const std::string & method) {
  CExpress::RequestType request_type;
  if (method == "GET") {
    request_type = CExpress::RequestType::GET;
  }
  else if (method == "HEAD") {
    request_type = CExpress::RequestType::HEAD;
  }
  else if (method == "POST") {
    request_type = CExpress::RequestType::POST;
  }
  else if (method == "PUT") {
    request_type = CExpress::RequestType::PUT;
  }
  else if (method == "DELET") {
    request_type = CExpress::RequestType::DELET;
  }
  else if (method == "CONNECT") {
    request_type = CExpress::RequestType::CONNECT;
  }
  else if (method == "OPTIONS") {
    request_type = CExpress::RequestType::OPTIONS;
  }
  else if (method == "TRACE") {
    request_type = CExpress::RequestType::TRACE;
  }
  else if (method == "PATCH") {
    request_type = CExpress::RequestType::PATCH;
  }
  else {
    throw "unable to determine request type";
  }
}