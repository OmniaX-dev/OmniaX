#ifndef __TEST_APP_HPP__
#define __TEST_APP_HPP__

#include <omniax/OmniaX.hpp>
#include <omniax/runtime/Application.hpp>

#define NORTH 0
#define SOUTH 1
#define EAST 2
#define WEST 3

struct Cell
{
    bool exists { false };
    uint32_t edge_index[4];
    bool has_edge[4];
};

struct Edge
{
    ox::Vec2 start;
    ox::Vec2 end;
};

class Ray
{
    public:
        inline Ray(void) { position = { 0, 0 }; direction = { 0, 0 }; }
        inline Ray(ox::Vec2 pos, ox::Vec2 dir) { position = pos; direction = dir; }
        void draw(void);
        ox::Vec2 cast(Edge edge);
        void updateDirection(const ox::Vec2& dir) { direction = dir; }

    public:
        ox::Vec2 position;
        ox::Vec2 direction;
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
    void onMousePressed(const ox::MouseButtonEvent& evt) override;
    void onMouseMoved(const ox::MouseMovedEvent& evt) override;

    void calculateVisibilityPolygon(float ox, float oy, float radius);
    void createEdgeList(int32_t sx, int32_t sy, int32_t w, int32_t h, float blockWidth, int32_t pitch);

private:
    ox::Camera2D camera;
    ox::ResourceID defaultShader;
    ox::RenderTexture shadowTexture;

    glm::mat4 model;
    ox::Vec2 tile_size { 32, 32 };
    std::vector<Cell> world;
    const uint32_t worldWidth { 40 };
    const uint32_t worldHeight { 30 };
    std::vector<Edge> edges;
    std::vector<std::tuple<float, float, float>> vecVisibilityPolygonPoints;
    ox::Vec2 mousePos;
    Ray ray;
};

#endif