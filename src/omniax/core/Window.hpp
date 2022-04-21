#ifndef __WINDOW_HPP__
#define __WINDOW_HPP__

#include <omniax/vendor/GLAD/glad/glad.h>
#include <GLFW/glfw3.h>
#include <omniax/utils/Types.hpp>
#include <omniax/utils/BaseObject.hpp>
#include <omniax/graphics/Color.hpp>
#include <omniax/utils/Defines.hpp>

class GLFWwindow;
namespace ox
{
    class Window : public BaseObject
	{
		public:
			typedef void(*fpCustomResizeCallback)(GLFWwindow*, int32_t, int32_t);

		public:
			inline Window(void) { invalidate(); }
			Window(int32_t width, int32_t height, String title, int32_t gl_major_version_hint = 4, int32_t gl_minor_version_hint = 2, int32_t gl_profile = GLFW_OPENGL_CORE_PROFILE);
			~Window(void);
			int32_t create(int32_t width, int32_t height, String title, int32_t gl_major_version_hint = 4, int32_t gl_minor_version_hint = 2, int32_t gl_profile = GLFW_OPENGL_CORE_PROFILE);
			void printOpenGLInfo(void);
			void processEvents(void);
			void close(void);
			bool isOpen(void);
			void clear(const Color& color, uint32_t gl_mask = GL_COLOR_BUFFER_BIT);
			void swapBuffers(void);
			void pollEvents(void);
			void setCustomResizeCallback(fpCustomResizeCallback callback);
			int32_t setWindowIcon(String path);

			inline int32_t gl_getMajorVersionHint(void) { return m_gl_majVer; }
			inline int32_t gl_getMinorVersionHint(void) { return m_gl_minVer; }
			inline int32_t gl_getProfileHint(void) { return m_gl_profile; }
			inline GLFWwindow* getGLFWWindowPtr(void) { return m_window; }

		private:
			static void framebuffer_size_callback(GLFWwindow* window, int32_t width, int32_t height);

		private:
			int32_t m_gl_majVer { 0 };
			int32_t m_gl_minVer { 0 };
			int32_t m_gl_profile { 0 };
			GLFWwindow* m_window { nullptr };
			fpCustomResizeCallback m_customResizeCallback { nullptr };

		public:
			inline static constexpr int32_t ERR_GLFW_WINDOW_FAILED = OX_WINDOW_ERR_MASK + 0x0001;
			inline static constexpr int32_t ERR_GLAD_INIT_FAILED = OX_WINDOW_ERR_MASK + 0x0002;
			inline static constexpr int32_t ERR_WINDOW_ICON_LOAD_FAILED = OX_WINDOW_ERR_MASK + 0x0003;
	};
}

#endif