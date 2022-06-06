#ifndef __TEST_APP_HPP__
#define __TEST_APP_HPP__

#include <omniax/OmniaX.hpp>
#include <omniax/runtime/Application.hpp>
#include <omniax/graphics/TileAnimation.hpp>

class TestApp : public ox::Application
{
    void onSetup(void) override;
    void onFrameStart(void) override;
    void onFrameEnd(void) override;
    void onRender(void) override;
    void onUpdate(void) override;
    void onImGuiRender(void) override;
    void onKeyPressed(const ox::KeyEvent& evt) override;
    void onMousePressed(const ox::MouseButtonEvent& evt) override;
    void onMouseReleased(const ox::MouseButtonEvent& evt) override;
    void onMouseMoved(const ox::MouseMovedEvent& evt) override;

private:
    ox::Camera2D camera;
    ox::ResourceID defaultShader;

    int32_t current { 0 };
    ox::ParticleEmitter snow;
    ox::ParticleEmitter fire;
    ox::ResourceID partTex;
    std::vector<ox::TextureAtlasIndex> partTiles;

    ox::ResourceID terrainTex;
    ox::ResourceID skyTex;
    ox::TextureAtlasIndex snowTile;
    ox::TextureAtlasIndex logsTile;
    
    // ox::RenderTarget lightMap;
    ox::RenderTarget scene;
    ox::RenderTarget gui;
    ox::ResourceID lightTex;
    // ox::ResourceID lightShad;
};

#endif