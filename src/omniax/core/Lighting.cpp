#include "Lighting.hpp"
#include <omniax/core/OrthoCamera.hpp>
#include <omniax/utils/Logger.hpp>

namespace ox
{
	void LightingManager::init(int32_t screen_width, int32_t screen_height)
	{
		s_renderTarget.create(screen_width, screen_height);
		LightingManager::s_screenWidth = screen_width;
		LightingManager::s_screenHeight = screen_height;
	}

	LightID LightingManager::addLightSource(tLightSource source)
	{
		if (source.texture.texture == ResourceManager::InvalidResource)
			source.texture.texture = ResourceManager::getDefaultLightTexture();
		LightingManager::s_lightSources[LightingManager::s_nextID] = source;
		return LightingManager::s_nextID++;
	}

	LightID LightingManager::addLightSource(Vec2 position, Vec2 size, Color color, TextureID texture)
	{
		return LightingManager::addLightSource({ position, size, texture, color });
	}
	
	tLightSource& LightingManager::getLightSource(LightID id)
	{
		if (LightingManager::s_lightSources.find(id) != LightingManager::s_lightSources.end())
			return LightingManager::s_lightSources[id];
		//OX_WARN("ox::LightingManager::getLightSource(...): Attempting to retrieve an invalid light.");
		return LightingManager::InvalidLightSource;
	}
	
	void LightingManager::update(void)
	{
		auto it = LightingManager::s_lightSources.begin();
		while (it != LightingManager::s_lightSources.end())
		{
			if (it->second.is_dead)
				it = LightingManager::s_lightSources.erase(it);
			else
				it++;
		}
	}
	
	void LightingManager::render(OrthoCamera& camera)
	{
		auto& rt = Renderer2D::getCurrentRenderTarget();
		Renderer2D::setRenderTarget(LightingManager::s_renderTarget);
		ox::Renderer2D::clear(LightingManager::s_ambientColor, GL_COLOR_BUFFER_BIT);
		for (auto& [id, light] : LightingManager::s_lightSources)
		{
			if (light.is_dead || !light.is_on) continue;
			ox::Renderer2D::drawQuad(ox::Renderer2D::getStaticQuad(light.position, light.size, true), light.color, light.texture.texture, light.texture.tile);
		}
		ResourceID shad = Renderer2D::getCurrentShaderID();
		ox::Renderer2D::setDefaultRenderTarget();
		auto& shader = Renderer2D::bindShader(ResourceManager::getDefaultLightShader());
		shader.updateUniform_i("u_lightPasses", 1);
		shader.updateUniform_mat4f("u_viewProjMatrix", camera.getViewProjectionMatrix());
		ox::Renderer2D::clear({ 0, 0, 0, 255 }, GL_COLOR_BUFFER_BIT);
		ox::Renderer2D::drawQuad(ox::Renderer2D::getStaticQuad({ 0, 0 }, { (float)LightingManager::s_screenWidth, (float)LightingManager::s_screenHeight }, false), { 255, 255, 255 }, rt.getResourceID());
		ox::Renderer2D::drawQuad(ox::Renderer2D::getStaticQuad({ 0, 0 }, { (float)LightingManager::s_screenWidth, (float)LightingManager::s_screenHeight }, false), { 255, 255, 255 }, LightingManager::s_renderTarget.getResourceID());
		Renderer2D::bindShader(shad);
		Renderer2D::setRenderTarget(rt);
	}
	
}