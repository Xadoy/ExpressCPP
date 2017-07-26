#pragma once

#include <functional>
#include <vector>
#include "request.h"
#include "response.h"
#include "server.h"
#include "route.h"

namespace CExpress {
  class Express
  {
  public:
    void use(std::function<void(CExpress::Request &, CExpress::Response &)>&);
    void route(std::string, std::string, std::function<void(CExpress::Request &, CExpress::Response &)>);
    void onerror(std::string, std::function<void(CExpress::Request &, CExpress::Response &)>);
    Express();
    void start(int port);

  private:
    std::vector<std::function<void(CExpress::Request &, CExpress::Response &)>> middleWares_;
    std::vector<std::pair<CExpress::Route, std::function<void(CExpress::Request &, CExpress::Response &)>>> controllers_;
    std::unordered_map<std::string, std::function<void(CExpress::Request &, CExpress::Response &)>> errorHandlers_;
    CExpress::Server server_;

    static const std::string DEFAULT_404_PAGE;
    static const std::string DEFAULT_500_PAGE;
	};
}
