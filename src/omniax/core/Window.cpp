#include "Window.hpp"
#include <omniax/utils/Logger.hpp>
#include <omniax/core/Errors.hpp>
#include <omniax/utils/Defines.hpp>
#include <omniax/utils/Utils.hpp>
#include <omniax/vendor/stb_image/stb_image.h>
#include <omniax/static_resources/default_icon_64.hpp>

namespace ox
{
    Window::Window(int32_t width, int32_t height, String title, int32_t gl_major_version_hint, int32_t gl_minor_version_hint, int32_t gl_profile)
    {
        create(width, height, title, gl_major_version_hint, gl_minor_version_hint, gl_profile);
    }

    Window::~Window(void)
    {
        glfwTerminate();
    }
    
    int32_t Window::create(int32_t width, int32_t height, String title, int32_t gl_major_version_hint, int32_t gl_minor_version_hint, int32_t gl_profile)
    {
        glfwInit();
        int32_t glmaj = 4;
        int32_t glmin = 6;
        m_window = nullptr;
        for (int32_t maj = glmaj; maj >= gl_major_version_hint; maj--)
        {
            for (int32_t min = glmin; min >= gl_minor_version_hint; min--)
            {
                glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, maj);
                glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, min);
                glfwWindowHint(GLFW_OPENGL_PROFILE, gl_profile);
                m_gl_profile = gl_profile;
                m_gl_minVer = min;
                m_gl_majVer = maj;
                m_window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
                if (m_window != nullptr)
                    break;
            }
        }
        if (m_window == nullptr)
        {
            ErrorHandler::pushError(Window::ERR_GLFW_WINDOW_FAILED);
            String error_str = ErrorHandler::getLastErrorString();
            OX_FATAL("%s.", error_str.c_str());
            glfwTerminate();
            return Window::ERR_GLFW_WINDOW_FAILED;
        }
        glfwMakeContextCurrent(m_window);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            ErrorHandler::pushError(Window::ERR_GLAD_INIT_FAILED);
            String error_str = ErrorHandler::getLastErrorString();
            OX_FATAL("%s.", error_str.c_str());
            glfwTerminate();
            return Window::ERR_GLAD_INIT_FAILED;
        }

        glViewport(0, 0, width, height);
        setCustomResizeCallback(nullptr);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_DEPTH_TEST);

        validate();
        setTypeName("ox::Window");
        return OX_NO_ERROR;
    }

    void Window::printOpenGLInfo(void)
    {
        OX_INFO("OpenGL Version: %s", glGetString(GL_VERSION));
        int nrAttributes;
        glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
        OX_INFO("Maximum number of vertex attributes supported: %d", nrAttributes);
        int nTexUnits;
        glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &nTexUnits);
        OX_INFO("Maximum number of Texture Units (per Stage): %d", nTexUnits);
        glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &nTexUnits);
        OX_INFO("Maximum number of Texture Units (Total): %d", nTexUnits);
    }

    void Window::processEvents(void)
    {
        if (glfwGetKey(m_window, GLFW_KEY_ESCAPE))
            glfwSetWindowShouldClose(m_window, true);
    }

    void Window::close(void) { glfwSetWindowShouldClose(m_window, true); }

    bool Window::isOpen(void) { return !glfwWindowShouldClose(m_window); }

    void Window::clear(const Color& color, uint32_t gl_mask) 
    {
        const auto fcol = color.getNormalizedColor();
        glClearColor(fcol.r, fcol.g, fcol.b, fcol.a);
        glClear(gl_mask);
    }

    void Window::swapBuffers(void) { glfwSwapBuffers(m_window); }

    void Window::pollEvents(void) { glfwPollEvents(); }

    void Window::setCustomResizeCallback(fpCustomResizeCallback callback)
    {
        m_customResizeCallback = callback;
        if (callback == nullptr)
            glfwSetWindowSizeCallback(m_window, &(Window::framebuffer_size_callback));
        else
            glfwSetWindowSizeCallback(m_window, m_customResizeCallback);
    }

    int32_t Window::setWindowIcon(String path)
    {
        GLFWimage images[1]; 
        if (StringEditor(path).trim().str() == "")
            images[0].pixels = stbi_load_from_memory(default_icon_64.data, default_icon_64.size, &images[0].width, &images[0].height, 0, 4);
        else
            images[0].pixels = stbi_load(path.c_str(), &images[0].width, &images[0].height, 0, 4);
        if (images[0].pixels == nullptr)
        {
            ErrorHandler::pushError(Window::ERR_WINDOW_ICON_LOAD_FAILED);
            String error_str = ErrorHandler::getLastErrorString();
            OX_WARN("%s\nImage path: <%s>", error_str.c_str(), path.c_str());
            return Window::ERR_WINDOW_ICON_LOAD_FAILED;
        }
        glfwSetWindowIcon(m_window, 1, images); 
        stbi_image_free(images[0].pixels);
        return OX_NO_ERROR;

    }
    
    void Window::framebuffer_size_callback(GLFWwindow* window, int32_t width, int32_t height)
    {
        glViewport(0, 0, width, height);
    }

}