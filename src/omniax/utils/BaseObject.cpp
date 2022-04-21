#include "BaseObject.hpp"
#include "Utils.hpp"
#include "Defines.hpp"

namespace ox
{
	BaseObject BaseObject::s_invalid_obj { false };

	std::ostream& operator<<(std::ostream& out, const BaseObject& val)
	{
		out << val.toString();
		return out;
	}

	void BaseObject::print(bool newLine, IOutputHandler* __destination) const
	{
		if (__destination == nullptr)
			std::cout << toString() << (newLine ? "\n" : "");
		else
		{
			__destination->p(toString());
			if (newLine) __destination->nl();
		}
	}

	std::string BaseObject::getObjectHeaderString(void) const
	{
		return StringEditor(getTypeName()).add("->uid=").addi(getID()).add("/oid=").addi(getCompareOID()).add("/valid=").add(STR_BOOL(isValid())).str();
	}
} //namesoace ox