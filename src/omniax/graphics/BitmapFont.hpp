#ifndef __BITMAP_FONT_HPP__
#define __BITMAP_FONT_HPP__

#include <omniax/utils/BaseObject.hpp>
#include <omniax/utils/Types.hpp>
#include <omniax/utils/Geometry.hpp>

namespace ox
{
	class BitmapFont : public BaseObject
	{
		public:
			inline BitmapFont(void) { invalidate(); }
			inline BitmapFont(String filePath) { create(filePath); }
			BitmapFont& create(String filePath);
			inline TextureAtlasIndex getChar(unsigned char c, Rectangle& outRect) { outRect = m_charBounds[(uint32_t)c]; return m_characters[(uint32_t)c]; }
			inline ResourceID getTexture(void) { return m_texture; }

		private:
			void calc_char_bounds(void);

		private:
			ResourceID m_texture;
			std::vector<TextureAtlasIndex> m_characters;
			std::vector<Rectangle> m_charBounds;
			Vec2 m_charSize;
			Vec2 m_textureSize;
	};
}

#endif