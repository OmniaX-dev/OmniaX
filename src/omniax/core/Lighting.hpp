#ifndef __LIGHTING_HPP__
#define __LIGHTING_HPP__

#include <omniax/utils/Geometry.hpp>
#include <omniax/graphics/Color.hpp>
#include <omniax/core/Renderer2D.hpp>
#include <omniax/core/ResourceManager.hpp>
#include <unordered_map>

namespace ox
{
	struct tLightSource
	{
		Vec2 position;
		Vec2 size;
		TextureID texture;
		Color color;
		bool is_on { true };
		bool is_dead { false };
	};

	class OrthoCamera;
	class LightingManager
	{
		public:
			static void init(int32_t screen_width, int32_t screen_height);
			static LightID addLightSource(tLightSource source);
			static LightID addLightSource(Vec2 position, Vec2 size, Color color, TextureID texture = { ResourceManager::InvalidResource, 0 });
			inline static void updateScreenSize(int32_t screen_width, int32_t screen_height) { LightingManager::s_screenWidth = screen_width; LightingManager::s_screenHeight = screen_height; }

			static tLightSource& getLightSource(LightID id);
			
			static void update(void);
			static void render(OrthoCamera& camera);

			inline static Color getAmbientColor(void) { return LightingManager::s_ambientColor; }
			inline static void setAnbientColor(Color color) { LightingManager::s_ambientColor = color; }
			inline static uint32_t getLightCount(void) { return LightingManager::s_lightSources.size(); }

		private:
			inline static std::unordered_map<LightID, tLightSource> s_lightSources;
			inline static LightID s_nextID = 1024;
			inline static Color s_ambientColor = { 255, 255, 255, 255 };
			inline static tLightSource InvalidLightSource = { { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0, 0, 0 } }; 
			inline static RenderTarget s_renderTarget;
			inline static int32_t s_screenWidth { 0 };
			inline static int32_t s_screenHeight { 0 };

		public:
			inline static const LightID InvalidLight = 0;

	};
}

#endif