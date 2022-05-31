#ifndef __TILE_ANIMATION_HPP__
#define __TILE_ANIMATION_HPP__

#include <omniax/utils/BaseObject.hpp>
#include <omniax/utils/Geometry.hpp>
#include <omniax/graphics/Color.hpp>
#include <omniax/core/DataStructures.hpp>

namespace ox
{
	class RenderTarget;
	class TileAnimation : public BaseObject
	{
		public:
			inline TileAnimation(void) { invalidate(); }
			TileAnimation(String name, ResourceID texture, std::vector<TextureAtlasIndex> frames = {}, int32_t delay = 10);
			void create(String name, ResourceID texture, std::vector<TextureAtlasIndex> frames = {}, int32_t delay = 10);

			void render(Vec2 position, Vec2 size, Transform2D transform = Transform2D(), const RenderTarget* target = nullptr);
			TextureAtlasIndex update(void);
			void resetAnimation(void);

			inline void playBackwards(bool b){ m_playBackwards = b; }
			inline void randomizeFrames(bool b) { m_randomFrames = b; }
			inline void setDelay(int32_t d){ m_delay = d; }
			inline void setStill(bool s, int32_t still_frame = 0){ m_isStill = s; m_stillFrame = still_frame; }
			inline void setName(String name) { m_name = name; }
			inline void setStillFrame(int32_t frame) { if (frame >= 0 && frame < length()) m_stillFrame = frame; }
			inline void setTexture(ResourceID texture) { m_texture = texture; }
			inline void addFrame(TextureAtlasIndex frame) { m_frames.push_back(frame); }
			inline void setFrames(std::vector<TextureAtlasIndex> frames) { m_frames = frames; }
			void setFrame(uint32_t frameIndex, TextureAtlasIndex frame);

			inline bool isPlayBackwardsEnabled(void){ return m_playBackwards; }
			inline int32_t getDelay(void){return m_delay;}
			inline bool isStill(void){return m_isStill;}
			inline String getName(void) { return m_name; }
			inline int32_t length(void) { return m_frames.size(); }
			inline int32_t getStillFrame(void) { return m_stillFrame; }
			inline bool isRandomEnabled(void) { return m_randomFrames; }
			inline ResourceID getTexture(void) { return m_texture; }
			inline std::vector<TextureAtlasIndex>& getFrames(void) { return m_frames; }

		private:
			int32_t m_current_time;
			int32_t m_current_frame;
			int32_t m_delay;
			int32_t m_stillFrame;

			bool m_isStill;
			bool m_playBackwards;
			bool m_randomFrames;
			bool m_back;

			ResourceID m_texture;
			String m_name;
			std::vector<TextureAtlasIndex> m_frames;

		private:
			inline static int32_t s_nextID = 0;
	};
}

#endif
