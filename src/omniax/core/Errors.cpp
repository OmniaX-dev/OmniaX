#include "Errors.hpp"
#include <omniax/utils/Utils.hpp>
#include <omniax/core/Window.hpp>
#include <omniax/core/Shader.hpp>
#include <omniax/utils/Defines.hpp>
#include <omniax/utils/Logger.hpp>
#include <omniax/core/GLBuffers.hpp>
#include <omniax/graphics/Texture.hpp>
#include <omniax/runtime/Application.hpp>
#include <omniax/graphics/BitmapFont.hpp>
#include <omniax/core/Renderer2D.hpp>

namespace ox
{
    String ErrorHandler::mapError(int32_t err_code)
    {
        StringEditor se;
        se.add(Utils::getHexStr(err_code, true, 4)).add(": ");
        switch (err_code)
        {
            case OX_NO_ERROR: return "";

            /** ox::Window Errors **/
            case Window::ERR_GLAD_INIT_FAILED: return se.add("Glad initalization failed.").str();
            case Window::ERR_GLFW_WINDOW_FAILED: return se.add("GLFW Window creation failed.").str();
            case Window::ERR_WINDOW_ICON_LOAD_FAILED: return se.add("Failed to set icon for GLFW Window.").str();

            /** ox::Shader Errors **/
            case Shader::ERR_SHADER_COMPILE_FAILED: return se.add("A Shader failed to compile").str();
            case Shader::ERR_SHADER_SOURCE_READ_FILED: return se.add("Failed to read shader source code").str();
            case Shader::ERR_FAILED_TO_REGISTER_UNIFORM: return se.add("Failed to register uniform").str();
            case Shader::ERR_FAILED_TO_UPDATE_UNIFORM: return se.add("Failed to update uniform").str();

            /** ox::VertexArray Errors **/
            case VertexArray::ERR_NO_EBO_SET: return se.add("No ElementBuffer set in VertexArray.").str();

            /** ox::Application Errors **/
            case Application::ERR_WINDOW_CREATE_FAIL: return se.add("Application failed to create ox::Window instance.").str();
            case Application::ERR_INVALID_APP_INSTANCE: return se.add("Invalid ox::Application reference in GLFW..").str();

            /** ox::Texture Errors **/
            case Texture::ERR_IMAGE_LOAD_FAILED: return se.add("Failed to load Texture.").str();
            case Texture::ERR_NO_DATA_STORED: return se.add("Attempting to retrieve local Texture data of a non-stored instance.").str();

            /** ox::BitmapFont Errors **/
            case BitmapFont::ERR_INVALID_TEXTURE: return se.add("Invalid BitmapFont Texture.").str();
            case BitmapFont::ERR_INVALID_TEXTURE_SIZE: return se.add("Invalid Size for ox::BitmapFont Texture: width and height must both be multiples of 16.").str();

            /** ox::Renderer2D::Text Errors **/
            case Renderer2D::Text::ERR_INVALID_BITMAPFONT: return se.add("Invalid BitmapFont.").str();

            /** ox::Renderer2D Errors **/
            case Renderer2D::ERR_INVALID_TEXTURE: return se.add("Invalid Texture.").str();
            case Renderer2D::ERR_INVALID_SHADER: return se.add("Invalid Shader.").str();

            /** ox::RenderTarget Errors **/
            case RenderTarget::ERR_FAILED_TO_CREATE_FRAMEBUFFER: return se.add("Failed to create FrameBuffer in ox::RenderTarget.").str();

            /** Custom or undefined Errors **/
            default:
            {
                if (ErrorHandler::s_customMapErrorCallback != nullptr)
                    return (*ErrorHandler::s_customMapErrorCallback)(err_code);
            }
        }
        return "Unknown Error.";
    }

    void ErrorHandler::pushError(int32_t err_code)
    {
        if (err_code != OX_NO_ERROR)
            s_errorStack.push_back(err_code);
    }

    String ErrorHandler::getLastErrorString(void)
    {
        if (ErrorHandler::s_errorStack.size() == 0) return "";
        return ErrorHandler::mapError(ErrorHandler::s_errorStack[s_errorStack.size() - 1]);
    }



    void ErrorHandler::__gl_clearErrors(void)
    {
        while(glGetError() != GL_NO_ERROR);
    }

    bool ErrorHandler::__gl_printError(const char* func_call, const char* file_path, int32_t line_number)
    {
        bool res = false;
        ox::StringEditor se;
        while (GLenum err_code = glGetError())
        {
            se = "[OpenGL]: (err_code = ";
            se.add(ox::Utils::getHexStr(err_code, true, 4)).add(")");
            OX_ERROR("%s:\nFile: %s on Line %d\n\t%s;\n", se.c_str(), file_path, line_number, func_call);
            res = true;
        }
        return res;
    }
}