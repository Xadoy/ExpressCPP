#include "response.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

using namespace CExpress;

Response::Response() {

}

void Response::setBodyWithFile(const std::string& file_name) {
  std::fstream file(file_name);
  if (!file.good()) {
    throw "unable to read file";
  }
  std::stringstream ss;
  std::string str = "";
  while (std::getline(file, str)) {
    ss << str << std::endl;
  }
  file.close();
  this->body = ss.str();
}
