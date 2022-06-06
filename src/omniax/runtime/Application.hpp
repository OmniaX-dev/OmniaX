#ifndef __APPLICATION_HPP__
#define __APPLICATION_HPP__

#include <omniax/core/Window.hpp>
#include <omniax/utils/Defines.hpp>
#include <omniax/runtime/Events.hpp>

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

			inline bool isRunning(void) { return isValid() && m_window.isOpen(); }
			inline uint32_t& getFps(void) { return m_fps; }
			inline void enableVSync(bool enabled = true) { glfwSwapInterval(enabled ? 1 : 0); }
			inline IPoint getWindowSize(void) { return m_window.getSize(); }
			inline int32_t getWindowWidth(void) { return m_window.getSize().x; }
			inline int32_t getWindowHeight(void) { return m_window.getSize().y; }

			void handleSignal(tSignal& signal) override;

			virtual void onFrameStart(void) {  }
			virtual void onFrameEnd(void) {  }
			virtual void onSetup(void) {  }
			virtual void onRender(void) {  }
			virtual void onUpdate(void) {  }
			virtual void onSecondsUpdate(void) {  }
			virtual void onImGuiRender(void) {  }
			virtual void onKeyPressed(const KeyEvent& evt) {  }
			virtual void onKeyReleased(const KeyEvent& evt) {  }
			virtual void onMousePressed(const MouseButtonEvent& evt) {  }
			virtual void onMouseReleased(const MouseButtonEvent& evt) {  }
			virtual void onMouseMoved(const MouseMovedEvent& evt) {  }
			virtual void onSignal(tSignal& signal) {  }

			BaseObject& getCustomData1(void);
			BaseObject& getCustomData2(void);
			BaseObject& getCustomData3(void);
			BaseObject& getCustomData4(void);

			inline void setCustomData1(BaseObject& data) { m_custom_data_1 = &data; }
			inline void setCustomData2(BaseObject& data) { m_custom_data_2 = &data; }
			inline void setCustomData3(BaseObject& data) { m_custom_data_3 = &data; }
			inline void setCustomData4(BaseObject& data) { m_custom_data_4 = &data; }

			inline void removeCustomData1(void) { m_custom_data_1 = nullptr; }
			inline void removeCustomData2(void) { m_custom_data_2 = nullptr; }
			inline void removeCustomData3(void) { m_custom_data_3 = nullptr; }
			inline void removeCustomData4(void) { m_custom_data_4 = nullptr; }

		private:
			static void __key_event_callback(GLFWwindow* _win, int32_t _key, int32_t _scan, int32_t _action, int32_t _mods);
			static void __mouse_btn_event_callback(GLFWwindow* _win, int32_t _btn, int32_t _action, int32_t _mods);
			static void __mouse_moved_callback(GLFWwindow* _win, double _x, double _y);


		protected:
			Window m_window;
			Color m_clearColor;

		private:
			BaseObject* m_custom_data_1 = nullptr;
			BaseObject* m_custom_data_2 = nullptr;
			BaseObject* m_custom_data_3 = nullptr;
			BaseObject* m_custom_data_4 = nullptr;

			uint32_t m_rtfps { 0 };
			uint32_t m_fps { 0 };
			double m_prevTime { 0.0 };
			double m_lastFrameTime { 0.0f };

		public:
			inline static constexpr int32_t ERR_WINDOW_CREATE_FAIL = OX_APPLICATION_ERR_MASK + 0x0001;
			inline static constexpr int32_t ERR_INVALID_APP_INSTANCE = OX_APPLICATION_ERR_MASK + 0x0002;
	};
}

#endif