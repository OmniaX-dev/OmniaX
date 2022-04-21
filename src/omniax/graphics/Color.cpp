#include "Color.hpp"
#include <cmath>

#include <omniax/utils/Utils.hpp>
#include <omniax/utils/Logger.hpp>

namespace ox
{
	Color::Color(void)
	{
		set();
		setTypeName("ox::Color");
		BaseObject::setValid(true);
	}
	
	Color::Color(uint8_t rgb_single_value, uint8_t alpha)
	{
		set(rgb_single_value, alpha);
		setTypeName("ox::Color");
		BaseObject::setValid(true);
	}
	
	Color::Color(uint8_t _r, uint8_t _g, uint8_t _b, uint8_t alpha)
	{
		set(_r, _g, _b, alpha);
		setTypeName("ox::Color");
		BaseObject::setValid(true);
	}
	
	// Color::Color(const sf::Color& sfml_color)
	// {
	// 	set(sfml_color);
	// 	setTypeName("ox::Color");
	// 	BaseObject::setValid(true);
	// }
	
	Color::Color(const String& color_string)
	{
		set(color_string);
		setTypeName("ox::Color");
		BaseObject::setValid(true);
	}
	
	Color::Color(const FloatCol& normalized_color)
	{
		set(normalized_color);
		setTypeName("ox::Color");
		BaseObject::setValid(true);
	}
	
	bool Color::operator==(const Color& col2)
	{
		return (r == col2.r && g == col2.g && b == col2.b && a == col2.a);
	}
	
	bool Color::operator!=(const Color& col2)
	{
		return !(*this == col2);
	}
	
	Color& Color::set(void)
	{
		r = 0;
		g = 0;
		b = 0;
		a = 255;
		return *this;
	}
	
	Color& Color::set(uint8_t rgb_single_value, uint8_t alpha)
	{
		r = rgb_single_value;
		g = rgb_single_value;
		b = rgb_single_value;
		a = alpha;
		return *this;
	}
	
	Color& Color::set(uint8_t _r, uint8_t _g, uint8_t _b, uint8_t alpha)
	{
		r = _r;
		g = _g;
		b = _b;
		a = alpha;
		return *this;
	}
	
	// Color& Color::set(const sf::Color& sfml_color)
	// {
	// 	r = sfml_color.r;
	// 	g = sfml_color.g;
	// 	b = sfml_color.b;
	// 	a = sfml_color.a;
	// 	return *this;
	// }
	
	Color& Color::set(const String& color_string)
	{
		StringEditor se(color_string);
		se.trim();
		r = g = b = 0;
		a = 255;
		if (se.startsWith("#"))
		{
			StringEditor tmp = se.substr(1);
			tmp.trim();
			se = "0x" + tmp.str();
		}
		if (se.startsWith("0x"))
		{
			int64_t ic = Utils::strToInt(se.str());
			union uC32 {
				uint8_t data[4];
				uint32_t value;
			} c32_u;
			c32_u.value = static_cast<uint32_t>(ic);
			a = c32_u.data[0];
			b = c32_u.data[1];
			g = c32_u.data[2];
			r = c32_u.data[3];
		}
		else if ((se.startsWith("(") || se.startsWith("rgba(") || se.startsWith("rgb(")) && se.endsWith(")") && se.contains(","))
		{
			se = se.substr(se.indexOf("(") + 1, se.len() - 1);
			se.trim();
			auto tokens = se.tokenize(",", true, false);
			if (tokens.count() < 3 || tokens.count() > 4)
			{
				OX_WARN("ox::Color::set(const String&) -> Invalid rgb string format: %s.", color_string.c_str());
				return *this;
			}
			r = Utils::strToInt(tokens.next());
			g = Utils::strToInt(tokens.next());
			b = Utils::strToInt(tokens.next());
			if (tokens.hasNext())
				a = Utils::strToInt(tokens.next());
		}
		else
		{
			OX_WARN("ox::Color::set(const String&) -> Unkown color string format: %s", color_string.c_str());
		}
		return *this;
	}
	
	Color& Color::set(const FloatCol& normalized_color)
	{
		r = static_cast<uint8_t>(std::round(normalized_color.r * 255));
		g = static_cast<uint8_t>(std::round(normalized_color.g * 255));
		b = static_cast<uint8_t>(std::round(normalized_color.b * 255));
		a = static_cast<uint8_t>(std::round(normalized_color.a * 255));
		return *this;
	}
	
	// sf::Color Color::sf(void) const
	// {
	// 	return { r, g, b, a };
	// }
	
	String Color::hexString(bool include_alpha, String prefix) const
	{
		String hex = "";
		hex += Utils::getHexStr(r, false, 1);
		hex += Utils::getHexStr(g, false, 1);
		hex += Utils::getHexStr(b, false, 1);
		if (include_alpha)
			hex += Utils::getHexStr(a, false, 1);
		hex = prefix + StringEditor(hex).toUpper().str();
		return hex;
	}
	
	String Color::rgbString(bool include_parenthesis, bool include_alpha) const
	{
		StringEditor rgb = "";
		if (include_parenthesis)
			rgb.add("(");
		rgb.addi(r).add(", ");
		rgb.addi(g).add(", ");
		rgb.addi(b);
		if (include_alpha)
			rgb.add(", ").addi(a);
		if (include_parenthesis)
			rgb.add(")");
		return rgb.str();
	}
	
	uint32_t Color::asInteger(void) const
	{
		union uC32 {
			uint8_t data[4];
			uint32_t value;
		} c32_u;
		c32_u.data[0] = a;
		c32_u.data[1] = b;
		c32_u.data[2] = g;
		c32_u.data[3] = r;
		return c32_u.value;
	}
	
	Color::FloatCol Color::getNormalizedColor(void) const
	{
		return { r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f };
	}
	
	String Color::toString(void) const
	{
		return hexString(true, "#") + " -> rgba" + rgbString(true, true);
	}
	
	void Color::print(bool newLine, IOutputHandler* __destination) const
	{
		if (__destination == nullptr)
			std::cout << *this << (newLine ? "\n" : "");
		else
		{
			__destination->p(this->toString());
			if (newLine) __destination->nl();
		}
	}
	
} //namesoace ox