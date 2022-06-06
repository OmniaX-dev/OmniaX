#include "Utils.hpp"
#include "Defines.hpp"
#include <omniax/vendor/TermColor.hpp>
#include <omniax/vendor/clip/clip.h>
#include <bitset>
#include <chrono>
#include <algorithm>
#include <cctype>
#include <string>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <thread>
#include <GLFW/glfw3.h>

namespace ox
{
	GameClock::GameClock(void)
	{
		minutes = 0;
		hours = 0;
		days = 0;
		months = 0;
		years = 2022;
		m_timeOfDay = 0.0f;
		m_totalSeconds = 0.0f;
	}

	const float& GameClock::start(void)
	{
		m_rtClock.start(false, "", eTimeUnits::Seconds);
		m_timeOfDay = CAP((1.0f / ((float)(TM_G_MINUTES_FOR_G_HOUR * TM_G_HOURS_FOR_G_DAY))) * ((hours * TM_G_MINUTES_FOR_G_HOUR) + (minutes)), 1.0f);
		return m_timeOfDay;
	}

	String GameClock::asString(void)
	{
		std::ostringstream ss;
		ss << "Time: " << getFormattedTime() << " - ";
		ss << (int32_t)(days + 1) << " " << convertMonth() << " " << (int32_t)(years);
		return String(ss.str());
	}

	void GameClock::update(void)
	{
		int64_t elapsed = m_rtClock.start(false, "", eTimeUnits::Seconds);
		if (hours == 255)
			hours = TM_G_HOURS_FOR_G_DAY - 1;
		else if (hours >= TM_G_HOURS_FOR_G_DAY)
			hours = 0;
		if (elapsed >= TM_R_SECONDS_FOR_G_MINUTE)
		{
			minutes++;
			if (minutes >= TM_G_MINUTES_FOR_G_HOUR)
			{
				hours++;
				if (hours >= TM_G_HOURS_FOR_G_DAY)
				{
					days++;
					if ((months == (uint8_t)eMonths::January || months == (uint8_t)eMonths::March ||
						months == (uint8_t)eMonths::May || months == (uint8_t)eMonths::July ||
						months == (uint8_t)eMonths::August || months == (uint8_t)eMonths::October ||
						months == (uint8_t)eMonths::December) && days >= TM_G_DAYS_FOR_G_LONG_MONTH)
					{
						months++;
						if (months > (uint8_t)eMonths::December)
						{
							years++;
							months = (uint8_t)eMonths::January;
						}
						days = 0;
					}
					else if ((months == (uint8_t)eMonths::April || months == (uint8_t)eMonths::June ||
							months == (uint8_t)eMonths::September || months == (uint8_t)eMonths::November) && days >= TM_G_DAYS_FOR_G_MEDIUM_MONTH)
					{
						months++;
						days = 0;
					}
					else if (months == (uint8_t)eMonths::February)
					{
						if ((years % 4 == 0 && days >= TM_G_DAYS_FOR_G_SHORT_MONTH + 1) ||
							(years % 4 != 0 && days >= TM_G_DAYS_FOR_G_SHORT_MONTH))
						{
							months++;
							days = 0;
						}
					}
					hours = 0;
				}
				minutes = 0;
			}
			m_totalSeconds += elapsed;
			m_rtClock.start(false, "", eTimeUnits::Seconds);
			m_timeOfDay = CAP((1.0f / ((float)(TM_G_MINUTES_FOR_G_HOUR * TM_G_HOURS_FOR_G_DAY))) * ((hours * TM_G_MINUTES_FOR_G_HOUR) + (minutes)), 1.0f);
		}
	}

	String GameClock::getFormattedTime(void)
	{
		bool zh = (int32_t)(hours / 10) < 1;
		bool zm = (int32_t)(minutes / 10) < 1;
		std::ostringstream ss;
		ss << (zh ? "0" : "") << (int32_t)hours << ":" << (zm ? "0" : "") << (int32_t)minutes;
		return String(ss.str());
	}

	String GameClock::convertMonth(void)
	{
		switch (months)
		{
			case (uint8_t)eMonths::January:
				return "January";
			case (uint8_t)eMonths::February:
				return "February";
			case (uint8_t)eMonths::March:
				return "March";
			case (uint8_t)eMonths::April:
				return "April";
			case (uint8_t)eMonths::May:
				return "May";
			case (uint8_t)eMonths::June:
				return "June";
			case (uint8_t)eMonths::July:
				return "July";
			case (uint8_t)eMonths::August:
				return "August";
			case (uint8_t)eMonths::September:
				return "September";
			case (uint8_t)eMonths::October:
				return "October";
			case (uint8_t)eMonths::November:
				return "November";
			case (uint8_t)eMonths::December:
				return "December";
			default:
				break;
		}
		return "_MONTH_";
	}
	
	#define __get_local_time() \
			std::time_t __cur_t = std::time(0); \
			std::tm* __now_t = std::localtime(&__cur_t);

	String LocalTime::getFullString(bool include_date, bool include_time, bool day_name, bool month_as_name, bool include_seconds) const
	{
		std::ostringstream ss;
		if (include_date)
		{
			if (day_name) ss << sWeekDay(true) << " ";
			ss << sday(true);
			if (month_as_name) ss << " " << smonth(false, true) << " ";
			else ss << "." << smonth(true, false) << ".";
			ss << syear();
		}
		if (include_time)
		{
			if (include_date) ss << " - ";
			ss << shours(true) << ":" << sminutes(true);
			if (include_seconds) ss << ":" << sseconds(true);
		}
		return ss.str();
	}

	int32_t LocalTime::hours(void) const
	{
		__get_local_time();
		return __now_t->tm_hour;
	}
	
	int32_t LocalTime::minutes(void) const
	{
		__get_local_time();
		return __now_t->tm_min;
	}
	
	int32_t LocalTime::seconds(void) const
	{
		__get_local_time();
		return __now_t->tm_sec;
	}
	
	int32_t LocalTime::day(void) const
	{
		__get_local_time();
		return __now_t->tm_mday;
	}
	
	int32_t LocalTime::month(void) const
	{
		__get_local_time();
		return __now_t->tm_mon + 1;
	}
	
	int32_t LocalTime::year(void) const
	{
		__get_local_time();
		return __now_t->tm_year + 1900;
	}
	
	int32_t LocalTime::weekDay(void) const
	{
		__get_local_time();
		return __now_t->tm_wday;
	}

	String LocalTime::shours(bool leading_zero) const
	{
		std::ostringstream ss;
		int32_t h = hours();
		if (leading_zero && h < 10)
			ss << "0" << (int32_t)h;
		else
			ss << (int32_t)h;
		return ss.str();
	}
	
	String LocalTime::sminutes(bool leading_zero) const
	{
		std::ostringstream ss;
		int32_t h = minutes();
		if (leading_zero && h < 10)
			ss << "0" << (int32_t)h;
		else
			ss << (int32_t)h;
		return ss.str();
	}
	
	String LocalTime::sseconds(bool leading_zero) const
	{
		std::ostringstream ss;
		int32_t h = seconds();
		if (leading_zero && h < 10)
			ss << "0" << (int32_t)h;
		else
			ss << (int32_t)h;
		return ss.str();
	}
	
	String LocalTime::sday(bool leading_zero) const
	{
		std::ostringstream ss;
		int32_t h = day();
		if (leading_zero && h < 10)
			ss << "0" << (int32_t)h;
		else
			ss << (int32_t)h;
		return ss.str();
	}
	
	String LocalTime::smonth(bool leading_zero, bool month_name) const
	{
		int32_t h = month();
		if (month_name) return monthToText(h);
		std::ostringstream ss;
		if (leading_zero && h < 10)
			ss << "0" << (int32_t)h;
		else
			ss << (int32_t)h;
		return ss.str();
	}
	
	String LocalTime::syear(void) const
	{
		std::ostringstream ss;
		int32_t h = year();
		ss << (int32_t)h;
		return ss.str();
	}

	String LocalTime::sWeekDay(bool day_name) const
	{
		int32_t h = weekDay();
		if (day_name)
			return weekDayToText(h);
		std::ostringstream ss;
		ss << (int32_t)h;
		return ss.str();
	}

	String LocalTime::monthToText(int32_t month) const
	{
		switch (month)
		{
			case 1: return "January";
			case 2: return "February";
			case 3: return "March";
			case 4: return "April";
			case 5: return "May";
			case 6: return "June";
			case 7: return "July";
			case 8: return "August";
			case 9: return "September";
			case 10: return "October";
			case 11: return "November";
			case 12: return "December";
			default: return "Unknown Month";
		}
	}

	String LocalTime::weekDayToText(int32_t day) const
	{
		switch (day)
		{
			case 0: return "Sun";
			case 1: return "Mon";
			case 2: return "Tue";
			case 3: return "Wed";
			case 4: return "Thu";
			case 5: return "Fri";
			case 6: return "Sat";
			default: return "Unknown Day";
		}
	}

	String LocalTime_IT::monthToText(int32_t month) const
	{
		switch (month)
		{
			case 1: return "Gennaio";
			case 2: return "Febraio";
			case 3: return "Marzo";
			case 4: return "Aprile";
			case 5: return "Maggio";
			case 6: return "Giugno";
			case 7: return "Luglio";
			case 8: return "Agosto";
			case 9: return "Settembre";
			case 10: return "Ottobre";
			case 11: return "Novembre";
			case 12: return "Dicembre";
			default: return "Mese sconosciuto";
		}
	}

	String LocalTime_IT::weekDayToText(int32_t day) const
	{
		switch (day)
		{
			case 0: return "Dom";
			case 1: return "Lun";
			case 2: return "Mar";
			case 3: return "Mer";
			case 4: return "Gio";
			case 5: return "Ven";
			case 6: return "Sab";
			default: return "Giorno Sconosciuto";
		}
	}

	String LocalTime_ES::monthToText(int32_t month) const
	{
		switch (month)
		{
			case 1: return "Enero";
			case 2: return "Febrero";
			case 3: return "Marzo";
			case 4: return "Abril";
			case 5: return "Mayo";
			case 6: return "Junio";
			case 7: return "Julio";
			case 8: return "Agosto";
			case 9: return "Septiembre";
			case 10: return "Octubre";
			case 11: return "Noviembre";
			case 12: return "Diciembre";
			default: return "Mes desconocido";
		}
	}

	String LocalTime_ES::weekDayToText(int32_t day) const
	{
		switch (day)
		{
			case 0: return "Domingo";
			case 1: return "Lunes";
			case 2: return "Martes";
			case 3: return "Miercoles";
			case 4: return "Jueves";
			case 5: return "Viernes";
			case 6: return "Sabado";
			default: return "Dia desconoscido";
		}
	}

	String LocalTime_DE::monthToText(int32_t month) const
	{
		switch (month)
		{
			case 1: return "Januar";
			case 2: return "Februar";
			case 3: return "Marz";
			case 4: return "April";
			case 5: return "May";
			case 6: return "Juni";
			case 7: return "July";
			case 8: return "August";
			case 9: return "September";
			case 10: return "Oktuber";
			case 11: return "November";
			case 12: return "Dizember";
			default: return "Unknown day";
		}
	}

	String LocalTime_DE::weekDayToText(int32_t day) const
	{
		switch (day)
		{
			case 0: return "So";
			case 1: return "Mo";
			case 2: return "Di";
			case 3: return "Mi";
			case 4: return "Do";
			case 5: return "Fr";
			case 6: return "Sa";
			default: return "Unknown day";
		}
	}



	uint64_t Timer::start(bool print, String name, eTimeUnits timeUnit, IOutputHandler* __destination)
	{
		m_timeUnit = timeUnit;
		m_started = true;
		m_name = name;
		if (print)
		{
			if (__destination == nullptr)
			{
				std::cout << "\n\n" << termcolor::magenta << "====>   ";
				std::cout << termcolor::cyan << "Starting test for [";
				std::cout << termcolor::green << m_name;
				std::cout << termcolor::cyan << "]";
				std::cout << termcolor::magenta << "   <====";
				std::cout << termcolor::reset << "\n";
			}
			else
			{
				m_dest = __destination;
				m_dest->nl().nl().col("magenta").p("====>   ");
				m_dest->col("cyan").p("Starting test for [");
				m_dest->col("green").p(m_name);
				m_dest->col("cyan").p("]");
				m_dest->nl().nl().col("magenta").p("   <====");
				m_dest->reset().nl();
			}
		}
		switch (m_timeUnit)
		{
			case eTimeUnits::Nanoseconds:
				m_current = std::chrono::duration_cast<std::chrono::nanoseconds> (std::chrono::system_clock::now().time_since_epoch()).count();
				return m_current;
			case eTimeUnits::Microseconds:
				m_current = std::chrono::duration_cast<std::chrono::microseconds> (std::chrono::system_clock::now().time_since_epoch()).count();
				return m_current;
			case eTimeUnits::Milliseconds:
				m_current = std::chrono::duration_cast<std::chrono::milliseconds> (std::chrono::system_clock::now().time_since_epoch()).count();
				return m_current;
			case eTimeUnits::Seconds:
				m_current = std::chrono::duration_cast<std::chrono::seconds> (std::chrono::system_clock::now().time_since_epoch()).count();
				return m_current;
			default: m_started = false; return 0;
		}
		m_started = false;
		return 0;
	}

	uint64_t Timer::end(bool print)
	{
		if (!m_started) return 0;
		m_started = false;
		m_dest = nullptr;
		int64_t diff;
		String unit;
		switch (m_timeUnit)
		{
			case eTimeUnits::Nanoseconds:
				diff = std::chrono::duration_cast<std::chrono::nanoseconds> (std::chrono::system_clock::now().time_since_epoch()).count();
				unit = " ns";
			break;
			case eTimeUnits::Microseconds:
				diff = std::chrono::duration_cast<std::chrono::microseconds> (std::chrono::system_clock::now().time_since_epoch()).count();
				unit = " us";
			break;
			case eTimeUnits::Milliseconds:
				diff = std::chrono::duration_cast<std::chrono::milliseconds> (std::chrono::system_clock::now().time_since_epoch()).count();
				unit = " ms";
			break;
			case eTimeUnits::Seconds:
				diff = std::chrono::duration_cast<std::chrono::seconds> (std::chrono::system_clock::now().time_since_epoch()).count();
				unit = " s";
			break;
			default: return 0;
		}
		diff -= m_current;
		if (print)
		{
			if (m_dest == nullptr)
			{
				std::cout << "\n" << termcolor::magenta << "====>   ";
				std::cout << termcolor::cyan << "Test for [";
				std::cout << termcolor::green << m_name;
				std::cout << termcolor::cyan << "] took ";
				std::cout << termcolor::bright_blue << diff << unit;
				std::cout << termcolor::magenta << "   <====";
				std::cout << termcolor::reset << "\n";
			}
			else
			{
				m_dest->nl().col("magenta").p("====>   ");
				m_dest->col("cyan").p("Test for [");
				m_dest->col("green").p(m_name);
				m_dest->col("cyan").p("] took ");
				m_dest->col("b-blue").pi(diff).p(unit);
				m_dest->nl().nl().col("magenta").p("   <====");
				m_dest->reset().nl();
			}
		}
		return diff;
	}



	void Utils::init(void)
	{
		Utils::s_startTime_ms = std::chrono::duration_cast<std::chrono::milliseconds>
								(std::chrono::system_clock::now().time_since_epoch()).count();
	}
	bool Utils::isHex(String hex)
	{
		hex = StringEditor(hex).trim().toLower().str();
		return hex.compare(0, 2, "0x") == 0 &&
				hex.size() > 2 &&
				hex.find_first_not_of("0123456789abcdef", 2) == std::string::npos;
	}
	bool Utils::isBin(String bin)
	{
		bin = StringEditor(bin).trim().toLower().str();
		return bin.compare(0, 2, "0b") == 0 &&
				bin.size() > 2 &&
				bin.find_first_not_of("01", 2) == std::string::npos;
	}
	bool Utils::isInt(String str)
	{
		str = StringEditor(str).trim().toLower().str();
		bool isNumber = std::ranges::all_of(str.begin(), str.end(),
                  [](char c){ return isdigit(c) != 0; });
		return Utils::isHex(str) || Utils::isBin(str) || isNumber;
	}
	int64_t Utils::strToInt(String str)
	{
		str = StringEditor(str).trim().toLower().str();
		if (!Utils::isInt(str)) return 0;
		int32_t base = 10;
		if (str.rfind("0x", 0) == 0)
		{
			str = str.substr(2);
			base = 16;
		}
		else if (str.rfind("0b", 0) == 0)
		{
			str = str.substr(2);
			base = 2;
		}
		return strtol(str.c_str(), NULL, base);
	}
	bool Utils::readFile(String fileName, std::vector<String>& outLines)
	{
		String line;
		std::ifstream file(fileName);
		if (file.fail()) return false;
		outLines.clear();
		while (std::getline(file, line))
			outLines.push_back(line);
		return true;
	}
	String Utils::getHexStr(uint64_t value, bool prefix, uint8_t nbytes)
	{
		union {
			uint64_t val;
			uint8_t bytes[8];
		} __tmp_editor;
		__tmp_editor.val = value;
		if (nbytes < 1 || nbytes > 8) nbytes = 1;
		std::ostringstream oss;
		if (prefix) oss << "0x";
		for (int8_t b = nbytes - 1; b >= 0; b--)
			oss << std::setw(2) << std::setfill('0') << std::uppercase << std::hex << (int)__tmp_editor.bytes[b];
		return oss.str();
	}
	String Utils::getBinStr(uint64_t value, bool prefix, uint8_t nbytes)
	{
		union {
			uint64_t val;
			uint8_t bytes[8];
		} __tmp_editor;
		__tmp_editor.val = value;
		if (nbytes < 1 || nbytes > 8) nbytes = 1;
		std::ostringstream oss;
		if (prefix) oss << "0b ";
		for (int8_t b = nbytes - 1; b >= 0; b--)
			oss << std::bitset<8>((char)__tmp_editor.bytes[b]) << " ";
		return oss.str();
	}
	String Utils::duplicateChar(unsigned char c, uint16_t count)
	{
		String str = "";
		for (uint16_t i = 0; i < count; i++)
			str = str += c;
		return str;
	}
	void Utils::sleep(uint32_t __time, eTimeUnits __unit)
	{
		switch (__unit)
		{
		case eTimeUnits::Seconds:
			std::this_thread::sleep_for(std::chrono::seconds(__time));
			break;
		case eTimeUnits::Milliseconds:
			std::this_thread::sleep_for(std::chrono::milliseconds(__time));
			break;
		case eTimeUnits::Microseconds:
			std::this_thread::sleep_for(std::chrono::microseconds(__time));
			break;
		case eTimeUnits::Nanoseconds:
			std::this_thread::sleep_for(std::chrono::nanoseconds(__time));
			break;
		default: break;
		}
	}
	uint64_t Utils::getRunningTime_ms(void)
	{
		return std::chrono::duration_cast<std::chrono::milliseconds> (std::chrono::system_clock::now().time_since_epoch()).count() - Utils::s_startTime_ms;
	}

	bool Utils::setClipboardText(String text)
	{
		return clip::set_text(text);
	}

	String Utils::getClipboardText(void)
	{
		String text = "";
		if (!clip::get_text(text)) return "";
		return text;
	}

	float Utils::map_value(float input, float input_start, float input_end, float output_start, float output_end)
	{
		float slope = 1.0 * (output_end - output_start) / (input_end - input_start);
		return output_start + round(slope * (input - input_start));
	}

	bool Utils::loadFileFromHppResource(String output_file_path, const char* resource_buffer, unsigned int size)
	{
		unsigned char ext_len = resource_buffer[0];
		String ext = "";
		for (unsigned char i = 0; i < ext_len; i++)
			ext += (char)(resource_buffer[i + 1]);
		if (StringEditor(output_file_path).trim().toLower().endsWith(ext))
			ext = "";
		std::fstream bin (output_file_path + ext, std::ios::out | std::ios::binary);
		if (!bin.is_open()) return false;
		bin.write(resource_buffer + ext_len + 1, size - ext_len - 1);
		bin.close();
		return true;
	}

	void Utils::printByteStream(const ByteStream& data, StreamIndex start, uint8_t line_len, uint16_t n_rows, IOutputHandler& out)
	{
		StreamIndex end = start + (n_rows * line_len);
		if (end > data.size()) end = data.size();
		uint8_t i = 1;
		ByteStream tmp;
		uint16_t linew = 1 + 1 + 6 + 1 + 1 + 2 + ((2 + 2) * line_len) + 1 + 4;
		out.col(ConsoleCol::BrightBlue).p(Utils::duplicateChar('=', linew)).nl();
		out.col(ConsoleCol::BrightBlue).p("| ");
		out.col(ConsoleCol::BrightGray).p("0x");
		out.col(ConsoleCol::BrightCyan).p(Utils::getHexStr(start, false, 4)).col(ConsoleCol::BrightBlue).p(" |  ");
		// std::cout << termcolor::bright_blue << Utils::duplicateChar('=', linew) << "\n";
		// std::cout << termcolor::bright_blue << "| ";
		// std::cout << termcolor::bright_grey << "0x";
		// std::cout << termcolor::bright_cyan << Utils::getHexStr(start, false, 4) << termcolor::bright_blue << " |  ";
		for (StreamIndex addr = start; addr < end; addr++)
		{
			tmp.push_back(data[addr]);
			if (data[addr] == 0)
				out.col(ConsoleCol::BrightGray);
				//std::cout << termcolor::bright_grey;
			else
				out.col(ConsoleCol::White);
				//std::cout << termcolor::bright_white;
			//std::cout << Utils::getHexStr(data[addr], false) << "  ";
			out.p(Utils::getHexStr(data[addr], false)).p("  ");
			if (i++ % line_len == 0 || addr == end - 1)
			{
				i = 1;
				//std::cout << termcolor::bright_blue << "|";
				out.col(ConsoleCol::BrightBlue).p("|");
				if (addr == end - 1) break;
				out.nl();
				out.col(ConsoleCol::BrightBlue).p("|");
				out.col(ConsoleCol::BrightGray).p("  --------  ").col(ConsoleCol::BrightBlue).p("|").col(ConsoleCol::BrightGray).p("  ");
				//std::cout << "\n";
				//std::cout << termcolor::bright_blue << "|";
				//std::cout << termcolor::bright_grey << "  --------  " << termcolor::bright_blue << "|" << termcolor::bright_grey << "  ";
				for (const auto& c : tmp)
				{
					//if (isprint(c)) std::cout << termcolor::bright_yellow << (char)c << termcolor::bright_grey << "   ";
					if (isprint(c)) out.col(ConsoleCol::BrightYellow).p((char)c).col(ConsoleCol::BrightGray).p("   ");
					else out.col(ConsoleCol::BrightGray).p(".   ");
					//else std::cout << termcolor::bright_grey << ".   ";
				}
				out.col(ConsoleCol::BrightBlue).p("| ");
				//std::cout << termcolor::bright_blue << "|";
				tmp.clear();
				out.reset().nl();
				out.col(ConsoleCol::BrightBlue).p("| ");
				out.col(ConsoleCol::BrightGray).p("0x");
				out.col(ConsoleCol::BrightCyan).p(Utils::getHexStr(addr + 1, false, 4)).col(ConsoleCol::BrightBlue).p(" |  ");
				//std::cout << termcolor::reset;
				//std::cout << "\n";
				// std::cout << termcolor::bright_blue << "| ";
				// std::cout << termcolor::bright_grey << "0x";
				// std::cout << termcolor::bright_cyan << Utils::getHexStr(addr + 1, false, 4) << termcolor::bright_blue << " |  ";
			}
		}
		out.nl().col(ConsoleCol::BrightBlue).p(Utils::duplicateChar('=', linew)).nl().reset();
		//std::cout << "\n" << termcolor::bright_blue << Utils::duplicateChar('=', linew) << "\n";
	}
} // namespace ox
