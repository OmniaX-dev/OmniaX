#include "Texture.hpp"
#include <omniax/core/Errors.hpp>
#include <omniax/vendor/stb_image/stb_image.h>
#include <omniax/utils/Logger.hpp>

namespace ox
{
	Texture::Texture(const String& path, bool store_data, int32_t min_filter_mode, int32_t mag_filter_mode, int32_t wrap_s_mode, int32_t wrap_t_mode)
	{
		create(path, store_data, min_filter_mode, mag_filter_mode, wrap_s_mode, wrap_t_mode);;
	}

	Texture& Texture::create(const String& path, bool store_data, int32_t min_filter_mode, int32_t mag_filter_mode, int32_t wrap_s_mode, int32_t wrap_t_mode)
	{
		invalidate();
		stbi_set_flip_vertically_on_load(1);
		if (m_localData != nullptr)
		{
			stbi_image_free(m_localData);
			m_width = 0;
			m_height = 0;
			m_localData = nullptr;
			m_filePath = "";
			m_bpp = 0;
			m_dataStored = false;
			setID(0);
		}
		m_localData = stbi_load(path.c_str(), &m_width, &m_height, &m_bpp, 4);
		if (m_localData == nullptr)
		{
			ErrorHandler::pushError(Texture::ERR_IMAGE_LOAD_FAILED);
			String err_str = ErrorHandler::getLastErrorString();
			OX_ERROR("%s\nFile Path: %s", err_str.c_str(), path.c_str());
			return *this;
		}
		uint32_t gl_id;
		GLCall(glGenTextures(1, &gl_id));
		setID(gl_id);
		bind();

		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter_mode));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag_filter_mode));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_s_mode));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_t_mode));

		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_localData));
		GLCall(glGenerateMipmap(GL_TEXTURE_2D));
		//unbind();

		m_dataStored = store_data;
		if (!m_dataStored)
		{
			stbi_image_free(m_localData);
			m_localData = nullptr;
		}

		m_filePath = path;
		setTypeName("ox::Texture");
		validate();
		return *this;
	}

	const uint8_t* Texture::getPixelData(void)
	{
		if (!hasDataStored())
		{
			ErrorHandler::pushError(Texture::ERR_NO_DATA_STORED);
			String err_str = ErrorHandler::getLastErrorString();
			OX_ERROR("%s", err_str.c_str());
			return nullptr;
		}
		return m_localData;
	}
	
	void Texture::bind(uint32_t slot) const
	{
		GLCall(glActiveTexture(GL_TEXTURE0 + slot));
		GLCall(glBindTexture(GL_TEXTURE_2D, getOpenGLID()));
	}
	
	void Texture::unbind(void) const
	{
		GLCall(glBindTexture(GL_TEXTURE_2D, 0));
	}
	
}