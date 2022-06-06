#include "TileAnimation.hpp"
#include <omniax/utils/Defines.hpp>
#include <omniax/graphics/Texture.hpp>
#include <omniax/core/ResourceManager.hpp>
#include <omniax/core/Renderer2D.hpp>
#include <omniax/utils/Random.hpp>
#include <iostream>

namespace ox
{
	TileAnimation::TileAnimation(String name, ResourceID texture, std::vector<TextureAtlasIndex> frames, int32_t delay)
	{
		create(name, texture, frames, delay);
	}

	void TileAnimation::create(String name, ResourceID texture, std::vector<TextureAtlasIndex> frames, int32_t delay)
	{
		m_current_time = 0;
		m_current_frame = 0;
		m_back = false;
		m_delay = delay;
		m_isStill = false;
		m_playBackwards = true;
		m_randomFrames = false;
		m_stillFrame = 1;
		m_frames = frames;
		m_texture = texture;
		m_name = name;
		setID(TileAnimation::s_nextID++);
		setTypeName("ox::TileAnimation");
		validate();
	}

	void TileAnimation::resetAnimation(void)
	{
		m_current_frame = 0;
		m_current_time = 0;
	}

	void TileAnimation::setFrame(uint32_t frameIndex, TextureAtlasIndex frame)
	{
		if (frameIndex < length())
			m_frames[frameIndex] = frame;
	}

	void TileAnimation::render(Vec2 position, Vec2 size, Transform2D transform, const RenderTarget* target)
	{
		if (length() == 0 || m_texture == ResourceManager::InvalidResource)
			return;
		const RenderTarget& rt = Renderer2D::getCurrentRenderTarget();
		if (target != nullptr)
			Renderer2D::setRenderTarget(*target);
		Renderer2D::drawQuad(position, size, m_texture, transform, m_frames[m_current_frame]);
		Renderer2D::setRenderTarget(rt);
	}

	TextureAtlasIndex TileAnimation::update(void)
	{
		if (length() == 0 || m_texture == ResourceManager::InvalidResource)
			return Texture::FullTextureCoords;
		if (m_current_time++ > m_delay && !m_isStill)
		{
			m_current_time = 0;
			if (m_randomFrames)
			{
				m_current_frame = Random::geti32(0, m_frames.size());
			}
			else if (m_playBackwards)
			{
				if (!m_back && m_current_frame >= m_frames.size() - 1)
				{
					m_back = true;
					m_current_frame--;
				}
				else if (!m_back)
				{
					m_current_frame++;
				}
				else if (m_back && m_current_frame <= 0)
				{
					m_back = false;
					m_current_frame++;
				}
				else if (m_back)
				{
					m_current_frame--;
				}
			}
			else
			{
				if (m_current_frame++ >= m_frames.size() - 1)
				{
					m_current_frame = 0;
				}
			}
		}
		else if (m_isStill)
			m_current_frame = m_stillFrame;

		return m_frames[m_current_frame];
	}
}
