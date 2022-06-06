#ifndef __RESOURCE_MANAGER_HPP__
#define __RESOURCE_MANAGER_HPP__

#include <omniax/utils/Types.hpp>
#include <unordered_map>
#include <omniax/core/Shader.hpp>
#include <omniax/graphics/Texture.hpp>
#include <omniax/graphics/BitmapFont.hpp>

namespace ox
{
	struct tResourceType
	{
		inline static constexpr uint8_t Invalid = 0;
		inline static constexpr uint8_t Shader = 1;
		inline static constexpr uint8_t Texture = 2;
		inline static constexpr uint8_t BitmapFont = 3;
	};

	class ResourceManager
	{
		public:
			static void init(void);
			static ResourceID loadShader(const String& vertex_file_path, const String& fragment_file_path);
			static ResourceID loadShader(const String& shader_name, bool single_file = true, const String& shader_folder = "");
			static ResourceID newShader(const String& vertex_shader_source, const String& fragment_shader_source);
			static ResourceID loadTexture(const String& filePath,
										bool store_data = false,
										int32_t min_filter_mode = GL_LINEAR,
										int32_t mag_filter_mode = GL_LINEAR,
										int32_t wrap_s_mode = GL_CLAMP_TO_EDGE,
										int32_t wrap_t_mode = GL_CLAMP_TO_EDGE);
			static ResourceID loadTexture(const unsigned char* data,
										  unsigned int data_size,
										  bool store_data = false,
										  int32_t min_filter_mode = GL_LINEAR,
										  int32_t mag_filter_mode = GL_LINEAR,
										  int32_t wrap_s_mode = GL_CLAMP_TO_EDGE,
										  int32_t wrap_t_mode = GL_CLAMP_TO_EDGE);
			static ResourceID newTexture(int32_t width, int32_t height);
			static ResourceID loadBitmapFont(const String& filePath);
			static ResourceID loadBitmapFont(const unsigned char* data, unsigned int data_size);

			static bool destroyResource(ResourceID resource, uint8_t res_type);

			static Shader& getShader(ResourceID id);
			static Texture& getTexture(ResourceID id);
			static BitmapFont& getBitmapFont(ResourceID id);

			inline static ResourceID getDefaultShader(void) { return ResourceManager::s_defaultShader; }
			inline static ResourceID getDefaultLightShader(void) { return ResourceManager::s_defaultLightShader; }
			inline static ResourceID getDefaultBitmapFont(void) { return ResourceManager::s_defaultBitmapFont; }
			inline static ResourceID getDefaultLightTexture(void) { return ResourceManager::s_defaultLightTexture; }

		private:
			inline static std::unordered_map<ResourceID, Shader> s_shaders;
			inline static std::unordered_map<ResourceID, Texture> s_textures; 
			inline static std::unordered_map<ResourceID, BitmapFont> s_bitmapFonts; 
			inline static ResourceID s_next_id = 1024;
			inline static ResourceID s_defaultShader { 0 };
			inline static ResourceID s_defaultLightShader { 0 };
			inline static ResourceID s_defaultBitmapFont { 0 };
			inline static ResourceID s_defaultLightTexture { 0 };
			inline static ResourceID s_ghostTexture { 0 };
			inline static ResourceID s_ghostShader { 0 };
			inline static ResourceID s_ghostBitmapFont { 0 };

		public:
			inline static const uint8_t InvalidResource = 0;
	};
} // namespace ox

#endif