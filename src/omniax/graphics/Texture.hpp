#ifndef __TEXTURE_HPP__
#define __TEXTURE_HPP__

#include <omniax/utils/BaseObject.hpp>
#include <omniax/utils/Types.hpp>
#include <omniax/vendor/GLAD/glad/glad.h>
#include <omniax/utils/Defines.hpp>

namespace ox
{
    class Texture : public BaseObject
    {
        public:
            inline Texture(void) { invalidate(); }
            Texture(const String& path,
                           bool store_data = false,
                           int32_t min_filter_mode = GL_LINEAR,
                           int32_t mag_filter_mode = GL_LINEAR,
                           int32_t wrap_s_mode = GL_CLAMP_TO_EDGE,
                           int32_t wrap_t_mode = GL_CLAMP_TO_EDGE);
            Texture& create(const String& path,
                           bool store_data = false,
                           int32_t min_filter_mode = GL_LINEAR,
                           int32_t mag_filter_mode = GL_LINEAR,
                           int32_t wrap_s_mode = GL_CLAMP_TO_EDGE,
                           int32_t wrap_t_mode = GL_CLAMP_TO_EDGE);
            const uint8_t* getPixelData(void);
            void bind(uint32_t slot = 0) const;
            void unbind(void) const;

			inline uint32_t getOpenGLID(void) const { return static_cast<uint32_t>(getID()); }
            inline String getFilePath(void) const { return m_filePath; }
            inline int32_t getWidth(void) const { return m_width; }
            inline int32_t getHeight(void) const { return m_height; }
            inline int32_t getBitsPerPixel(void) const { return m_bpp; }
            inline bool hasDataStored(void) const { return isValid() && m_dataStored; }

        private:
            String m_filePath;
            uint8_t* m_localData { nullptr };
            int32_t m_width { 0 };
            int32_t m_height { 0 };
            int32_t m_bpp { 0 };
            bool m_dataStored { false };

        public:
            inline static constexpr int32_t ERR_IMAGE_LOAD_FAILED = OX_TEXTURE_ERR_MASK + 0x0001;
            inline static constexpr int32_t ERR_NO_DATA_STORED = OX_TEXTURE_ERR_MASK + 0x0002;
    };
}

#endif