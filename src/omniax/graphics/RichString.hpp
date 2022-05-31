#ifndef __RICH_STRING_HPP__
#define __RICH_STRING_HPP__

#include <omniax/graphics/Color.hpp>

namespace ox
{
	struct tRichChar
	{
		unsigned char ascii { 0 };
		Color foreground { 0, 0 };
		Color background { 0, 0 };
	};

	class RichString
	{
		public:
			inline RichString(void) {  }
			inline String getText(void) { return m_text; }
			tRichChar at(uint32_t index);
			void add(tRichChar rchar);
			void add(String str, Color fg, Color bg);
			void add(String str);

		private:
			String m_text { "" };
			std::vector<Color> m_foreground;
			std::vector<Color> m_background; 
	};
}

#endif