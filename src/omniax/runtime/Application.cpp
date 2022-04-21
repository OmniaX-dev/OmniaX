#include "Application.hpp"
#include <omniax/core/Renderer.hpp>
#include <omniax/vendor/imgui/imgui.h>
#include <omniax/vendor/imgui/imgui_impl_glfw.h>
#include <omniax/vendor/imgui/imgui_impl_opengl3.h>
#include <omniax/utils/Utils.hpp>
#include <omniax/utils/Logger.hpp>
#include <omniax/core/Errors.hpp>

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
        if (StringEditor(windowIconPath).trim().str() != "")
            m_window.setWindowIcon(windowIconPath);

        /** ImGui Setup **/
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        ImGui::StyleColorsDark();
        ImGui_ImplGlfw_InitForOpenGL(m_window.getGLFWWindowPtr(), true);
        StringEditor se = "#version ";
        se.addi(gl_major_version_hint).addi(gl_minor_version_hint).add("0");
        ImGui_ImplOpenGL3_Init(se.c_str());

        ox::Renderer::init();

        m_windowWidth = windowWidth;
        m_windowHeight = windowHeight;

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
        ox::Renderer::shutdown();
    }

    void Application::nextFrame(void)
    {
        onFrameStart();
        m_window.clear(m_clearColor, m_gl_clear_bit_mask);
        m_window.processEvents();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ox::Renderer::resetStats();

        onUpdate();
        onRender();
        onImGuiRender();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        m_window.swapBuffers();
        m_window.pollEvents();
        onFrameEnd();
    }
}