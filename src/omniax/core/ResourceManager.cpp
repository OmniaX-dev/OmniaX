#include "ResourceManager.hpp"

namespace ox //TODO: Errors
{
	ResourceID ResourceManager::loadShader(const String& vertex_file_path, const String& fragment_file_path)
	{
		ResourceManager::s_shaders[ResourceManager::s_next_id].createFromSeparateFiles(vertex_file_path, fragment_file_path);
		if (ResourceManager::s_shaders[ResourceManager::s_next_id].isInvalid())
		{
			return ResourceManager::InvalidResource;
		}
		return ResourceManager::s_next_id++;
	}

	ResourceID ResourceManager::loadShader(const String& shader_name, bool single_file, const String& shader_folder)
	{
		ResourceManager::s_shaders[ResourceManager::s_next_id].createFromName(shader_name, single_file, shader_folder);
		if (ResourceManager::s_shaders[ResourceManager::s_next_id].isInvalid())
		{
			return ResourceManager::InvalidResource;
		}
		return ResourceManager::s_next_id++;
	}

	ResourceID ResourceManager::loadTexture(const String& filePath, bool store_data, int32_t min_filter_mode, int32_t mag_filter_mode, int32_t wrap_s_mode, int32_t wrap_t_mode)
	{
		ResourceManager::s_textures[ResourceManager::s_next_id].create(filePath, store_data, min_filter_mode, mag_filter_mode, wrap_s_mode, wrap_t_mode);
		if (ResourceManager::s_textures[ResourceManager::s_next_id].isInvalid())
		{
			return ResourceManager::InvalidResource;
		}
		return ResourceManager::s_next_id++;
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
} // namespace ox