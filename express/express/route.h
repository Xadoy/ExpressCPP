#pragma once

#include <functional>

namespace CExpress {
	class Route;
}

namespace std
{
	template <>
	class hash<CExpress::Route>
	{
	public:
		size_t operator() (const CExpress::Route& r) const;
	};
}

namespace CExpress {
	class Route {
		friend size_t std::hash<Route>::operator() (const Route&) const;
	};
}

