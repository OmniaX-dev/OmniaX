#include "Application.hpp"
#include <omniax/core/Renderer2D.hpp>
#include <omniax/vendor/imgui/imgui.h>
#include <omniax/vendor/imgui/imgui_impl_glfw.h>
#include <omniax/vendor/imgui/imgui_impl_opengl3.h>
#include <omniax/utils/Utils.hpp>
#include <omniax/utils/Logger.hpp>
#include <omniax/core/Errors.hpp>
#include <omniax/runtime/Signals.hpp>
#include <omniax/core/ResourceManager.hpp>
#include <omniax/core/Input.hpp>

namespace ox
{
	Application& Application::create(int32_t windowWidth, int32_t windowHeight, String windowTitle, String windowIconPath, int32_t gl_major_version_hint, int32_t gl_minor_version_hint, int32_t gl_profile)
	{
		/** Window Code **/
		m_window.create(windowWidth, windowHeight, windowTitle, gl_major_version_hint, gl_minor_version_hint, gl_profile);
		if (!m_window.isValid())
		{
			ErrorHandler::pushError(Application::ERR_WINDOW_CREATE_FAIL);
			String err_str = ErrorHandler::getLastErrorString();
			OX_FATAL("%s", err_str.c_str());
			return *this;
		}
#ifdef __DEBUG__
		m_window.printOpenGLInfo();
#endif
		m_window.setWindowIcon(windowIconPath);

		/** Subsystem initialization **/
		ox::Renderer2D::init();
		SignalHandler::init();
		ResourceManager::init();
		Input::init(m_window.getGLFWWindowPtr());
		/******************************/

		m_windowWidth = windowWidth;
		m_windowHeight = windowHeight;

		glfwSetWindowUserPointer(m_window.getGLFWWindowPtr(), static_cast<void*>(this));
		glfwSetKeyCallback(m_window.getGLFWWindowPtr(), __key_event_callback);
		glfwSetMouseButtonCallback(m_window.getGLFWWindowPtr(), __mouse_btn_event_callback);
		glfwSetCursorPosCallback(m_window.getGLFWWindowPtr(), __mouse_moved_callback);
		enableVSync(true);

		/** ImGui Setup **/
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		ImGui::StyleColorsDark();
		ImGui_ImplGlfw_InitForOpenGL(m_window.getGLFWWindowPtr(), true);
		StringEditor se = "#version ";
		se.addi(gl_major_version_hint).addi(gl_minor_version_hint).add("0");
		ImGui_ImplOpenGL3_Init(se.c_str());

		setTypeName("ox::Application");
		validate();
		onSetup();
		return *this;
	}

	void Application::shutDown(void)
	{
		/** Program End **/
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
		ox::Renderer2D::shutdown();
	}

	void Application::nextFrame(void)
	{
		double currTime = glfwGetTime();
		m_rtfps++;
		if (currTime - m_prevTime >= 1.0f)
		{
			m_fps = m_rtfps;
			m_rtfps = 0;
			m_prevTime = currTime;
		}
		onFrameStart();
		m_window.clear(m_clearColor, m_gl_clear_bit_mask);
		m_window.processEvents();
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ox::Renderer2D::resetStats();

		onUpdate();
		onRender();
		onImGuiRender();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		SignalHandler::refresh();
		m_window.swapBuffers();
		m_window.pollEvents();
		onFrameEnd();
	}

	BaseObject& Application::getCustomData1(void)
	{
		if (m_custom_data_1 == nullptr) return BaseObject::InvalidRef();
		return *m_custom_data_1;
	}

	BaseObject& Application::getCustomData2(void)
	{
		if (m_custom_data_1 == nullptr) return BaseObject::InvalidRef();
		return *m_custom_data_2;
	}

	BaseObject& Application::getCustomData3(void)
	{
		if (m_custom_data_1 == nullptr) return BaseObject::InvalidRef();
		return *m_custom_data_3;
	}

	BaseObject& Application::getCustomData4(void)
	{
		if (m_custom_data_1 == nullptr) return BaseObject::InvalidRef();
		return *m_custom_data_4;
	}

	void Application::__key_event_callback(GLFWwindow* _win, int32_t _key, int32_t _scan, int32_t _action, int32_t _mods)
	{
		Application* app = static_cast<Application*>(glfwGetWindowUserPointer(_win));
		if (app == nullptr) return; //TODO: Error
		if (_action == GLFW_PRESS)
		{
			auto evt = KeyEvent(app->m_window, *app, tEventTypes::KeyPressed, _key, true);
			SignalHandler::emitSignal(tBuiltinSignals::KeyPressed, tSignalPriority::RealTime, evt);
			app->onKeyPressed(evt);
		}
		else if (_action == GLFW_RELEASE) 
		{
			auto evt = KeyEvent(app->m_window, *app, tEventTypes::KeyReleased, _key, false);
			SignalHandler::emitSignal(tBuiltinSignals::KeyReleased, tSignalPriority::RealTime, evt);
			app->onKeyReleased(evt);
		}
	}

	void Application::__mouse_btn_event_callback(GLFWwindow* _win, int32_t _btn, int32_t _action, int32_t _mods)
	{
		Application* app = static_cast<Application*>(glfwGetWindowUserPointer(_win));
		if (app == nullptr) return; //TODO: Error
		double xpos, ypos;
		glfwGetCursorPos(_win , &xpos, &ypos);
		uint32_t mx = static_cast<uint32_t>(xpos);
		uint32_t my = static_cast<uint32_t>(ypos);
		if (_action == GLFW_PRESS)
		{
			auto evt = MouseButtonEvent(app->m_window, *app, tEventTypes::MousePressed, static_cast<uint8_t>(_btn), true, mx, my);
			SignalHandler::emitSignal(tBuiltinSignals::MousePressed, tSignalPriority::RealTime, evt);
			app->onMousePressed(evt);
		}
		else if (_action == GLFW_RELEASE)
		{
			auto evt = MouseButtonEvent(app->m_window, *app, tEventTypes::MouseReleased, static_cast<uint8_t>(_btn), true, mx, my);
			SignalHandler::emitSignal(tBuiltinSignals::MouseReleased, tSignalPriority::RealTime, evt);
			app->onMouseReleased(evt);
		}
	}

	void Application::__mouse_moved_callback(GLFWwindow* _win, double _x, double _y)
	{
		Application* app = static_cast<Application*>(glfwGetWindowUserPointer(_win));
		if (app == nullptr) return; //TODO: Error
		if (_x < 0) _x = 0;
		if (_y < 0) _y = 0;
		//TODO: Check for other window bounds
		uint32_t mx = static_cast<uint32_t>(_x);
		uint32_t my = static_cast<uint32_t>(_y);
		auto evt = MouseMovedEvent(app->m_window, *app, mx, my);
		SignalHandler::emitSignal(tBuiltinSignals::MouseMoved, tSignalPriority::RealTime, evt);
		app->onMouseMoved(evt);
	}
}