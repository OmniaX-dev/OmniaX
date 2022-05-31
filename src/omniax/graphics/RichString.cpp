#include "RichString.hpp"

namespace ox
{
	tRichChar RichString::at(uint32_t index)
	{
		if (index >= m_text.size()) return tRichChar(); //TODO: Error
		return { (unsigned char)m_text[index], m_foreground[index], m_background[index] };
	}

	void RichString::add(tRichChar rchar)
	{
		m_text += rchar.ascii;
		m_foreground.push_back(rchar.foreground);
		m_background.push_back(rchar.background);
	}

	void RichString::add(String str, Color fg, Color bg)
	{
		for (auto& c : str)
		{
			m_text += c;
			m_foreground.push_back(fg);
			m_background.push_back(bg);
		}
	}

	void RichString::add(String str)
	{
		Color fcol(255);
		Color bcol(0, 0);
		if (m_text.length() > 0)
		{
			fcol = m_foreground[m_foreground.size() - 1];
			bcol = m_background[m_background.size() - 1];
		}
		add(str, fcol, bcol);
	}
}