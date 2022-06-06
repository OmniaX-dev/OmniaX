#include "BitmapFont.hpp"
#include <omniax/core/ResourceManager.hpp>
#include <omniax/graphics/Texture.hpp>
#include <omniax/core/Errors.hpp>
#include <omniax/utils/Logger.hpp>

namespace ox
{
	BitmapFont& BitmapFont::create(String filePath)
	{
		m_texture = ResourceManager::loadTexture(filePath, true, GL_LINEAR, GL_LINEAR);
		__load();
		return *this;
	}

	BitmapFont& BitmapFont::create(ResourceID texture)
	{
		m_texture = texture;
		__load();
		return *this;
	}

	void BitmapFont::calc_char_bounds(void)
	{
		uint32_t xtiles = m_textureSize.x / m_charSize.x;
		uint32_t ytiles = m_textureSize.y / m_charSize.y;
		const auto& pixels = ox::ResourceManager::getTexture(m_texture).getPixelData();
		for (int32_t y = ytiles - 1; y >= 0; y--)
		{
			for (int32_t x = 0; x < xtiles; x++)
			{
				int32_t xx = (x * m_charSize.x * 4);
				int32_t yy = (y * m_charSize.y * 4);
				float bl = m_charSize.x, bt = m_charSize.y, br = 0.0f, bb = 0.0f;
				int32_t revy = 0;
				for (int32_t ty = m_charSize.y - 1; ty >= 0; ty--, revy++)
				{
					for (int32_t tx = 0; tx < m_charSize.x; tx++)
					{
						uint32_t px = xx + (tx * 4);
						uint32_t py = yy + (ty * 4);
						uint8_t alpha = pixels[CONVERT_2D_1D(px, py, (uint32_t)m_textureSize.x)];
						if (alpha != 0)
						{
							if (revy < m_charSize.y / 2)
							{	if (revy < bt) bt = revy;	}
							else if (revy > bb) bb = revy;

							if (tx < m_charSize.x / 2)
							{	if (tx < bl) bl = tx;	}
							else if (tx > br) br = tx;
						}
					}
				}
				Rectangle rect(bl - 2, bt - 2, br - bl + 4, bb - bt + 4);
				if (rect.x < 0) rect.x = 0;
				if (rect.y < 0) rect.y = 0;
				if (rect.w < 1) rect.w = 1;
				if (rect.h < 1) rect.h = 1;
				m_charBounds.push_back(rect);
			}
		}
	}

	void BitmapFont::__load(void)
	{
		if (m_texture == ResourceManager::InvalidResource)
		{
			ErrorHandler::pushError(BitmapFont::ERR_INVALID_TEXTURE);
			String err_str = ErrorHandler::getLastErrorString();
			OX_ERROR("%s", err_str.c_str());
			return;
		}
		auto& tex = ResourceManager::getTexture(m_texture);
		if (tex.getWidth() % 16 != 0 || tex.getHeight() % 16 != 0)
		{
			ErrorHandler::pushError(BitmapFont::ERR_INVALID_TEXTURE_SIZE);
			String err_str = ErrorHandler::getLastErrorString();
			OX_ERROR("%s", err_str.c_str());
			return;
		}
		m_charSize = { (float)(tex.getWidth() / 16), (float)(tex.getHeight() / 16) };
		m_textureSize = { (float)tex.getWidth(), (float)tex.getHeight() };
		calc_char_bounds();
		m_charBounds[(uint32_t)' '] = { (m_charSize.x - m_spaceWidth) / 2.0f, 5.0f, m_spaceWidth, m_charSize.y - 10 };
		for (uint32_t y = 0; y < 16; y++)
		{
			for (uint32_t x = 0; x < 16; x++)
			{
				auto& bounds = m_charBounds[CONVERT_2D_1D(x, y, 16)];
				m_characters.push_back(tex.addTileInfo((x * m_charSize.x) + bounds.x,
													   (y * m_charSize.y) + bounds.y,
													    bounds.w,
														bounds.h));
			}
		}
		setTypeName("ox::BitmapFont");
		validate();
	}
}