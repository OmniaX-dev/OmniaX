#include "ResourceManager.hpp"
#include <omniax/static_resources/default_shaders.hpp>

namespace ox //TODO: Errors
{
	void ResourceManager::init(void)
	{
		ResourceManager::s_defaultShader = ResourceManager::newShader(defaultShader_vert, defaultShader_frag);
	}

	ResourceID ResourceManager::loadShader(const String& vertex_file_path, const String& fragment_file_path)
	{
		auto id = (ResourceManager::s_ghostShader != 0 ? ResourceManager::s_ghostShader : ResourceManager::s_next_id++);
		ResourceManager::s_shaders[id].createFromSeparateFiles(vertex_file_path, fragment_file_path);
		if (ResourceManager::s_shaders[id].isInvalid())
		{
			ResourceManager::s_ghostShader = id;
			return ResourceManager::InvalidResource;
		}
		ResourceManager::s_ghostShader = 0;
		return id;
	}

	ResourceID ResourceManager::loadShader(const String& shader_name, bool single_file, const String& shader_folder)
	{
		auto id = (ResourceManager::s_ghostShader != 0 ? ResourceManager::s_ghostShader : ResourceManager::s_next_id++);
		ResourceManager::s_shaders[id].createFromName(shader_name, single_file, shader_folder);
		if (ResourceManager::s_shaders[id].isInvalid())
		{
			ResourceManager::s_ghostShader = id;
			return ResourceManager::InvalidResource;
		}
		ResourceManager::s_ghostShader = 0;
		return id;
	}

	ResourceID ResourceManager::newShader(const String& vertex_shader_source, const String& fragment_shader_source)
	{
		auto id = (ResourceManager::s_ghostShader != 0 ? ResourceManager::s_ghostShader : ResourceManager::s_next_id++);
		ResourceManager::s_shaders[id].createFromSources(vertex_shader_source, fragment_shader_source);
		if (ResourceManager::s_shaders[id].isInvalid())
		{
			ResourceManager::s_ghostShader = id;
			return ResourceManager::InvalidResource;
		}
		ResourceManager::s_ghostShader = 0;
		return id;
	}

	ResourceID ResourceManager::loadTexture(const String& filePath, bool store_data, int32_t min_filter_mode, int32_t mag_filter_mode, int32_t wrap_s_mode, int32_t wrap_t_mode)
	{
		auto id = (ResourceManager::s_ghostTexture != 0 ? ResourceManager::s_ghostTexture : ResourceManager::s_next_id++);
		ResourceManager::s_textures[id].create(filePath, store_data, min_filter_mode, mag_filter_mode, wrap_s_mode, wrap_t_mode);
		if (ResourceManager::s_textures[id].isInvalid())
		{
			ResourceManager::s_ghostTexture = id;
			return ResourceManager::InvalidResource;
		}
		ResourceManager::s_ghostTexture = 0;
		return id;
	}

	ResourceID ResourceManager::newTexture(int32_t width, int32_t height)
	{
		auto id = (ResourceManager::s_ghostTexture != 0 ? ResourceManager::s_ghostTexture : ResourceManager::s_next_id++);
		ResourceManager::s_textures[id].create(width, height);
		if (ResourceManager::s_textures[id].isInvalid())
		{
			ResourceManager::s_ghostTexture = id;
			return ResourceManager::InvalidResource;
		}
		ResourceManager::s_ghostTexture = 0;
		return id;
	}

	ResourceID ResourceManager::loadBitmapFont(const String& filePath)
	{
		auto id = (ResourceManager::s_ghostBitmapFont != 0 ? ResourceManager::s_ghostBitmapFont : ResourceManager::s_next_id++);
		ResourceManager::s_bitmapFonts[id].create(filePath);
		if (ResourceManager::s_bitmapFonts[id].isInvalid())
		{
			ResourceManager::s_ghostBitmapFont = id;
			return ResourceManager::InvalidResource;
		}
		ResourceManager::s_ghostBitmapFont = 0;
		return id;
	}

	Shader& ResourceManager::getShader(ResourceID id)
	{
		if (ResourceManager::s_shaders.find(id) == ResourceManager::s_shaders.end())
			return (Shader&)BaseObject::InvalidRef();
		return ResourceManager::s_shaders[id];
	}
	
	Texture& ResourceManager::getTexture(ResourceID id)
	{
		if (ResourceManager::s_textures.find(id) == ResourceManager::s_textures.end())
			return (Texture&)BaseObject::InvalidRef();
		return ResourceManager::s_textures[id];
	}

	BitmapFont& ResourceManager::getBitmapFont(ResourceID id)
	{
		if (ResourceManager::s_bitmapFonts.find(id) == ResourceManager::s_bitmapFonts.end())
			return (BitmapFont&)BaseObject::InvalidRef();
		return ResourceManager::s_bitmapFonts[id];
	}
} // namespace ox