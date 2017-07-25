#pragma once

#include <functional>
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
		std::vector<std::function<void(CExpress::Request &, CExpress::Response &)>> _middleWares;
		std::unordered_map<CExpress::Route, std::function<void(CExpress::Request &, CExpress::Response &)>> _controllers;
		std::unordered_map<std::string, std::function<void(CExpress::Request &, CExpress::Response &)>> _errorHandlers;
		CExpress::Server _server;
	};
}