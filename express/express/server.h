#pragma once

#include <string>
#include <memory>
#include <functional>

namespace CExpress {
	class Server {
	public:
		Server();
		void setPort(int);
		void process(std::function<std::string (std::string)>);
	};
}

