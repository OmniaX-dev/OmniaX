#include "Geometry.hpp"

#include <iostream>
#include "Utils.hpp"

namespace ox
{
	String Vec2::toString(void) const
	{
		return StringEditor("{ ").addf(x).add(", ").addf(y).add(" }").str();
	}
} //namesoace ox