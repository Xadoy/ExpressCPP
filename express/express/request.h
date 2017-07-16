#pragma once

#include <unordered_map>

namespace CExpress {
	class Request {
	public:
		std::unordered_map<std::string, std::string> Parameter;
		std::unordered_map<std::string, std::string> Query;
		std::string Body;
		std::unordered_map<std::string, std::string> Header;

		Request();
	};
}