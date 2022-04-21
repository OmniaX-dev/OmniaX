#ifndef __TEST_APP_HPP__
#define __TEST_APP_HPP__

#include <omniax/OmniaX.hpp>
#include <omniax/runtime/Application.hpp>

class TestApp : public ox::Application
{
    void onSetup(void) override;
    void onFrameStart(void) override;
    void onFrameEnd(void) override;
    void onRender(void) override;
    void onUpdate(void) override;
    void onImGuiRender(void) override;

private:
    ox::Camera2D camera;
    ox::Shader baseShader;
    ox::Texture omniaxLogo;
    ox::Texture omniaxLogo_i;
    glm::mat4 model;
    ox::Vec2 tile_size { 16, 16 };
    ox::Timer timer;
    ox::Timer timer2;
    uint64_t sum = 0;
    uint64_t ms = 0;
    float fps = 0;
};

#endif