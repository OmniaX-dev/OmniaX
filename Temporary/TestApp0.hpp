#ifndef __TEST_APP_HPP__
#define __TEST_APP_HPP__

#include <omniax/OmniaX.hpp>
#include <omniax/runtime/Application.hpp>
#include <omniax/runtime/Signals.hpp>

class TestObj : public ox::SignalRecieverObject
{
    public:
        void handleSignal(ox::tSignal& signal) override;
};

class TestApp : public ox::Application
{
    void onSetup(void) override;
    void onFrameStart(void) override;
    void onFrameEnd(void) override;
    void onRender(void) override;
    void onUpdate(void) override;
    void onImGuiRender(void) override;
    void onKeyPressed(const ox::KeyEvent& evt) override;
    void onKeyReleased(const ox::KeyEvent& evt) override;
    void onMousePressed(const ox::MouseButtonEvent& evt) override;
    void onMouseReleased(const ox::MouseButtonEvent& evt) override;
    void onMouseMoved(const ox::MouseMovedEvent& evt) override;

private:
    ox::Camera2D camera;
    ox::ResourceID baseShader;
    ox::ResourceID baseShader2;
    ox::ResourceID texAtlas;
    ox::TextureAtlasIndex omniaxLogo;
    ox::TextureAtlasIndex omniaxLogo_i;
    glm::mat4 model;
    ox::Vec2 tile_size { 16, 16 };
    ox::FrameInterpolator<ox::Color> colInterp1 { { 255, 0, 0 }, {0, 255, 0}, 60 };
    ox::FrameInterpolator<ox::Color> colInterp2 { { 0, 255, 0 }, {0, 0, 255}, 60 };
    ox::FrameInterpolator<ox::Color> colInterp3 { { 0, 0, 255 }, {255, 0, 0}, 60 };
    ox::FrameInterpolatorChain<ox::Color> fic;

    TestObj obj;
};

#endif