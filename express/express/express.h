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
    void use(const std::function<void(CExpress::Request &, CExpress::Response &)>&);
    void route(const std::string&, const std::string&, const std::function<void(CExpress::Request &, CExpress::Response &)>&);
    void onerror(const std::string&, const std::function<void(CExpress::Request &, CExpress::Response &)>&);
    Express();
    void start(int port);

  private:
    std::vector<std::function<void(CExpress::Request &, CExpress::Response &)>> middlewares_;
    std::vector<std::pair<CExpress::Route, std::function<void(CExpress::Request &, CExpress::Response &)>>> controllers_;
    std::unordered_map<std::string, std::function<void(CExpress::Request &, CExpress::Response &)>> error_handlers_;
    CExpress::Server server_;

    void handle500(Request &, Response &, const char * errorMsg = nullptr) const;
    void handle404(Request &, Response &) const;

    // set parameters with pathTokens and route
    void setRquestParameters(Request&, const std::vector<std::string>&, const std::vector<std::string>&);

    void setRequestQueryParameters(Request & request, const std::string & attributes);

    void setRequestHeaders(Request & request, std::string & request_header);

    static const std::string DEFAULT_404_PAGE;
    static const std::string DEFAULT_500_PAGE;
    static const std::string STATUS_CODE_200;
    static const std::string STATUS_CODE_404;
    static const std::string STATUS_CODE_500;
	};
}
