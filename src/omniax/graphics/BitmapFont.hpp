#ifndef __BITMAP_FONT_HPP__
#define __BITMAP_FONT_HPP__

#include <omniax/utils/BaseObject.hpp>
#include <omniax/utils/Types.hpp>
#include <omniax/utils/Geometry.hpp>
#include <omniax/utils/Defines.hpp>

namespace ox
{
	class BitmapFont : public BaseObject
	{
		public:
			inline BitmapFont(void) { invalidate(); }
			inline BitmapFont(String filePath) { create(filePath); }
			BitmapFont& create(String filePath);
			BitmapFont& create(ResourceID texture);
			inline TextureAtlasIndex getChar(unsigned char c, Rectangle& outRect) { outRect = m_charBounds[(uint32_t)c]; return m_characters[(uint32_t)c]; }
			inline ResourceID getTexture(void) { return m_texture; }
			inline float getBaseCharHeight(void) { return m_charSize.y; }
			inline float getSpaceWidth(void) { return m_spaceWidth; }
			inline void setSpaceWidth(float w) { m_spaceWidth = w; }

		private:
			void calc_char_bounds(void);
			void __load(void);

		private:
			ResourceID m_texture;
			std::vector<TextureAtlasIndex> m_characters;
			std::vector<Rectangle> m_charBounds;
			Vec2 m_charSize;
			Vec2 m_textureSize;
			float m_spaceWidth { 20.0f };

		
		public:
			inline static constexpr int32_t ERR_INVALID_TEXTURE = OX_BITMAPFONT_ERR_MASK + 0x0001;
			inline static constexpr int32_t ERR_INVALID_TEXTURE_SIZE = OX_BITMAPFONT_ERR_MASK + 0x0002;
	};
}

#endif