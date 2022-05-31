#ifndef __TEXTURE_HPP__
#define __TEXTURE_HPP__

#include <omniax/utils/BaseObject.hpp>
#include <omniax/utils/Types.hpp>
#include <omniax/vendor/GLAD/glad/glad.h>
#include <omniax/utils/Defines.hpp>
#include <omniax/utils/Geometry.hpp>

namespace ox
{
    class Texture : public BaseObject
    {
        public: struct tTexCoords
        {
            Vec2 topLeft { 0.0f, 1.0f };
            Vec2 topRight { 1.0f, 1.0f };
            Vec2 bottomRight { 1.0f, 0.0f };
            Vec2 bottomLeft { 0.0f, 0.0f };
        };
        public:
            inline Texture(void) { invalidate(); }
            Texture(const String& path,
                           bool store_data = false,
                           int32_t min_filter_mode = GL_LINEAR,
                           int32_t mag_filter_mode = GL_LINEAR,
                           int32_t wrap_s_mode = GL_CLAMP_TO_EDGE,
                           int32_t wrap_t_mode = GL_CLAMP_TO_EDGE);
            Texture(int32_t width, int32_t height);
            Texture& create(const String& path,
                           bool store_data = false,
                           int32_t min_filter_mode = GL_LINEAR,
                           int32_t mag_filter_mode = GL_LINEAR,
                           int32_t wrap_s_mode = GL_CLAMP_TO_EDGE,
                           int32_t wrap_t_mode = GL_CLAMP_TO_EDGE);
            Texture& create(int32_t width, int32_t height);
            const uint8_t* getPixelData(void);
            void bind(uint32_t slot = 0) const;
            void unbind(void) const;

			inline uint32_t getOpenGLID(void) const { return static_cast<uint32_t>(getID()); }
            inline String getFilePath(void) const { return m_filePath; }
            inline int32_t getWidth(void) const { return m_width; }
            inline int32_t getHeight(void) const { return m_height; }
            inline int32_t getBitsPerPixel(void) const { return m_bpp; }
            inline bool hasDataStored(void) const { return isValid() && m_dataStored; }
            inline bool hasTileData(void) { return m_tiles.size() > 0; }
            TextureAtlasIndex addTileInfo(uint32_t x, uint32_t y, uint32_t w, uint32_t h);
            tTexCoords getTile(TextureAtlasIndex index);

        private:
            String m_filePath;
            uint8_t* m_localData { nullptr };
            int32_t m_width { 0 };
            int32_t m_height { 0 };
            int32_t m_bpp { 0 };
            bool m_dataStored { false };
            std::vector<tTexCoords> m_tiles;

        public:
            inline static constexpr int32_t ERR_IMAGE_LOAD_FAILED = OX_TEXTURE_ERR_MASK + 0x0001;
            inline static constexpr int32_t ERR_NO_DATA_STORED = OX_TEXTURE_ERR_MASK + 0x0002;

            inline static constexpr TextureAtlasIndex FullTextureCoords = 0;
    };
}

#endif