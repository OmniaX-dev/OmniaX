#include "Utils.hpp"
#include <omniax/graphics/Color.hpp>
#include "BaseObject.hpp"
#include "Logger.hpp"
#include <omniax/vendor/TermColor.hpp>

#include <iomanip>
#include <iostream>

namespace ox
{
	IOutputHandler& ConsoleOutputHandler::col(String color)
	{
		StringEditor cse = color;
		cse.toLower().trim();
		if (cse.startsWith("@[") && cse.endsWith("]"))
		{
			OX_WARN("ox::ConsoleOutputHandler::col(ox::String): RGB Colors not supported for standard console output: %s", cse.c_str());
			return *this;
		}

		//RED
		if (cse.str() == "red") std::cout << termcolor::red;
		else if (cse.str() == "b-red") std::cout << termcolor::bright_red;
		else if (cse.str() == "o-red") std::cout << termcolor::on_red;
		else if (cse.str() == "ob-red") std::cout << termcolor::on_bright_red;
		//GREEN
		else if (cse.str() == "green") std::cout << termcolor::green;
		else if (cse.str() == "b-green") std::cout << termcolor::bright_green;
		else if (cse.str() == "o-green") std::cout << termcolor::on_green;
		else if (cse.str() == "ob-green") std::cout << termcolor::on_bright_green;
		//BLUE
		else if (cse.str() == "blue") std::cout << termcolor::blue;
		else if (cse.str() == "b-blue") std::cout << termcolor::bright_blue;
		else if (cse.str() == "o-blue") std::cout << termcolor::on_blue;
		else if (cse.str() == "ob-blue") std::cout << termcolor::on_bright_blue;
		//MAGENTA
		else if (cse.str() == "magenta") std::cout << termcolor::magenta;
		else if (cse.str() == "b-magenta") std::cout << termcolor::bright_magenta;
		else if (cse.str() == "o-magenta") std::cout << termcolor::on_magenta;
		else if (cse.str() == "ob-magenta") std::cout << termcolor::on_bright_magenta;
		//CYAN
		else if (cse.str() == "cyan" || cse.str() == "aqua") std::cout << termcolor::cyan;
		else if (cse.str() == "b-cyan") std::cout << termcolor::bright_cyan;
		else if (cse.str() == "o-cyan" || cse.str() == "o-cyan") std::cout << termcolor::on_cyan;
		else if (cse.str() == "ob-cyan") std::cout << termcolor::on_bright_cyan;
		//YELLOW
		else if (cse.str() == "yellow") std::cout << termcolor::yellow;
		else if (cse.str() == "b-yellow") std::cout << termcolor::bright_yellow;
		else if (cse.str() == "o-yellow") std::cout << termcolor::on_yellow;
		else if (cse.str() == "ob-yellow") std::cout << termcolor::on_bright_yellow;
		//GRAY
		else if (cse.str() == "gray" || cse.str() == "black") std::cout << termcolor::grey;
		else if (cse.str() == "b-gray") std::cout << termcolor::bright_grey;
		else if (cse.str() == "o-gray" || cse.str() == "o-black") std::cout << termcolor::on_grey;
		else if (cse.str() == "ob-gray") std::cout << termcolor::on_bright_grey;
		//WHITE
		else if (cse.str() == "lgray") std::cout << termcolor::white;
		else if (cse.str() == "white" || cse.str() == "b-lgray") std::cout << termcolor::bright_white;
		else if (cse.str() == "o-lgray") std::cout << termcolor::on_white;
		else if (cse.str() == "ob-lgray" || cse.str() == "o-white") std::cout << termcolor::on_bright_white;
		else
			OX_WARN("ox::ConsoleOutputHandler::col(ox::String): Unknown color string: %s", cse.c_str());
		return *this;
	}

	IOutputHandler& ConsoleOutputHandler::col(const Color& color)
	{
		StringEditor cse = "@[";
		cse.addi(color.r).add(", ");
		cse.addi(color.g).add(", ");
		cse.addi(color.b).add(", ");
		cse.addi(color.a).add("]");
		return col(cse.str());
	}
	
	IOutputHandler& ConsoleOutputHandler::p(char c)
	{
		std::cout << (char)c;
		return *this;
	}
	
	IOutputHandler& ConsoleOutputHandler::p(const StringEditor& se)
	{
		std::cout << se;
		return *this;
	}
	
	IOutputHandler& ConsoleOutputHandler::pi(uint8_t i)
	{
		std::cout << (int64_t)i;
		return *this;
	}
	
	IOutputHandler& ConsoleOutputHandler::pi(int8_t i)
	{
		std::cout << (int64_t)i;
		return *this;
	}
	
	IOutputHandler& ConsoleOutputHandler::pi(uint16_t i)
	{
		std::cout << (int64_t)i;
		return *this;
	}
	
	IOutputHandler& ConsoleOutputHandler::pi(int16_t i)
	{
		std::cout << (int64_t)i;
		return *this;
	}
	
	IOutputHandler& ConsoleOutputHandler::pi(uint32_t i)
	{
		std::cout << (int64_t)i;
		return *this;
	}
	
	IOutputHandler& ConsoleOutputHandler::pi(int32_t i)
	{
		std::cout << (int64_t)i;
		return *this;
	}
	
	IOutputHandler& ConsoleOutputHandler::pi(uint64_t i)
	{
		std::cout << (uint64_t)i;
		return *this;
	}
	
	IOutputHandler& ConsoleOutputHandler::pi(int64_t i)
	{
		std::cout << (int64_t)i;
		return *this;
	}
	
	IOutputHandler& ConsoleOutputHandler::pf(float f, uint8_t precision)
	{
		if (precision == 0)
		{
			std::cout << f;
		}
		else
		{
			std::ios cout_state(nullptr);
			std::cout << std::fixed << std::setprecision(precision) << f;
			std::cout.copyfmt(cout_state);
		}
		return *this;
	}
	
	IOutputHandler& ConsoleOutputHandler::pf(double f, uint8_t precision)
	{
		if (precision == 0)
		{
			std::cout << f;
		}
		else
		{
			std::ios cout_state(nullptr);
			std::cout << std::fixed << std::setprecision(precision) << f;
			std::cout.copyfmt(cout_state);
		}
		return *this;
	}
	
	IOutputHandler& ConsoleOutputHandler::nl(void)
	{
		std::cout << "\n";
		return *this;
	}
	
	IOutputHandler& ConsoleOutputHandler::flush(void)
	{
		std::cout << std::endl;
		return *this;
	}
	
	IOutputHandler& ConsoleOutputHandler::reset(void)
	{
		std::cout << termcolor::reset;
		return *this;
	}
}