#ifndef __RESOURCE_MANAGER_HPP__
#define __RESOURCE_MANAGER_HPP__

#include <omniax/utils/Types.hpp>
#include <unordered_map>
#include <omniax/core/Shader.hpp>
#include <omniax/graphics/Texture.hpp>

namespace ox
{
	class ResourceManager
	{
		public:
			static ResourceID loadShader(const String& vertex_file_path, const String& fragment_file_path);
			static ResourceID loadShader(const String& shader_name, bool single_file = true, const String& shader_folder = "");
			static ResourceID loadTexture(const String& filePath,
										bool store_data = false,
										int32_t min_filter_mode = GL_LINEAR,
										int32_t mag_filter_mode = GL_LINEAR,
										int32_t wrap_s_mode = GL_CLAMP_TO_EDGE,
										int32_t wrap_t_mode = GL_CLAMP_TO_EDGE);

			static Shader& getShader(ResourceID id);
			static Texture& getTexture(ResourceID id);

		private:
			inline static std::unordered_map<ResourceID, Shader> s_shaders;
			inline static std::unordered_map<ResourceID, Texture> s_textures; 
			inline static ResourceID s_next_id = 1024;

		public:
			inline static constexpr uint8_t InvalidResource = 0;
	};
} // namespace ox

#endif