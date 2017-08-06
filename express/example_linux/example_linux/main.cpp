//
//  main.cpp
//  example_linux
//
//  Created by Tony Li on 2017-08-04.
//  Copyright © 2017 AlephTeam. All rights reserved.
//

#include <iostream>

#include "express.h"

int main(int argc, const char * argv[]) {
  using namespace CExpress;
  Express ex;
  ex.route("GET", "/", [=](Request & req, Response & res) {
    res.body = "<h1>Hello World</h1>";
    res.headers["Content-type"] = "text/html";
  });
  ex.start(8081);
}
