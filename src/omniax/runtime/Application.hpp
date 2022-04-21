#ifndef __APPLICATION_HPP__
#define __APPLICATION_HPP__

#include <omniax/core/Window.hpp>
#include <omniax/utils/Defines.hpp>

namespace ox
{
	class Application : public BaseObject
	{
		public:
			inline Application(void) { invalidate(); }
			inline Application(int32_t windowWidth, int32_t windowHeight, String windowTitle, String windowIconPath = "", int32_t gl_major_version_hint = 4, int32_t gl_minor_version_hint = 2, int32_t gl_profile = GLFW_OPENGL_CORE_PROFILE) { create(windowWidth, windowHeight, windowTitle, windowIconPath, gl_major_version_hint, gl_minor_version_hint, gl_profile); }
			Application& create(int32_t windowWidth, int32_t windowHeight, String windowTitle, String windowIconPath = "", int32_t gl_major_version_hint = 4, int32_t gl_minor_version_hint = 2, int32_t gl_profile = GLFW_OPENGL_CORE_PROFILE);
			void shutDown(void);
			void nextFrame(void);
			inline void addOpenGLClearBit(uint32_t gl_clear_bit) { m_gl_clear_bit_mask |= gl_clear_bit; }

			inline bool isRunning(void) { return isValid() && m_window.isOpen(); }

			virtual void onFrameStart(void) {  }
			virtual void onFrameEnd(void) {  }
			virtual void onSetup(void) {  }
			virtual void onRender(void) {  }
			virtual void onUpdate(void) {  }
			virtual void onImGuiRender(void) {  }


		protected:
			Window m_window;
			Color m_clearColor;
            int32_t m_windowWidth { 0 }; //TODO: Make theese dynamic (probably in the ox::Window class)
            int32_t m_windowHeight { 0 };

		private:
			uint32_t m_gl_clear_bit_mask { GL_COLOR_BUFFER_BIT };

		public:
			inline static constexpr int32_t ERR_WINDOW_CREATE_FAIL = OX_APPLICATION_ERR_MASK + 0x0001;
	};
}

#endif