#include "TestApp.hpp"
//#include <omniax/vendor/imgui/imgui_demo.cpp>

void TestApp::onSetup(void)
{
    camera.create(0.0f, m_windowWidth * 1.0f, m_windowHeight * 1.0f, 0.0f);
    baseShader.createFromName("baseShader", true);
    omniaxLogo.create("res/icon512.png");
    omniaxLogo_i.create("res/icon512_2.png");
    model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
    baseShader.bind();
    int32_t samplers[16];
    for (uint8_t i = 0; i < 16; i++)
        samplers[i] = i;
    glUniform1iv(baseShader.gl_getUniformLocation("u_textures"), 16, samplers);
    addOpenGLClearBit(GL_DEPTH_BUFFER_BIT);
}

void TestApp::onFrameStart(void)
{
    timer.start(false, "", ox::eTimeUnits::Milliseconds);
    timer2.start(false, "", ox::eTimeUnits::Milliseconds);
}

void TestApp::onFrameEnd(void)
{
    ms = timer.end(false);
    sum += timer2.end(false);
    if (sum > 500)
    {
        sum = 0;
        fps = (1 / (ms / 1000.0f));
    }
}

void TestApp::onRender(void)
{
    baseShader.bind();
    baseShader.updateUniform_mat4f("u_viewProjMatrix", camera.getViewProjectionMatrix());
    baseShader.updateUniform_mat4f("u_modelMatrix", model);

    ox::Renderer::beginBatch();
    for (size_t y = 0; y < m_windowHeight / tile_size.y; y++)
    {
        for (size_t x = 0; x < m_windowWidth / tile_size.x; x++)
        {
            ox::Renderer::drawQuad({ x * tile_size.x, y * tile_size.y }, { tile_size.x, tile_size.y }, (((x + y) % 2) == 0 ? omniaxLogo : omniaxLogo_i)); 
        }
    }
    ox::Renderer::endBatch();
    ox::Renderer::flush();
}

void TestApp::onUpdate(void)
{
    
}

void TestApp::onImGuiRender(void)
{
    //ImGui::ShowDemoWindow();
    ImGui::Begin("Controls");
        ImGui::LabelText("", "DrawCalls: %d", ox::Renderer::getRenderStats().drawCalls);
        ImGui::LabelText("", "Quads: %d", ox::Renderer::getRenderStats().quadCount);
        ImGui::LabelText("", "FPS: %f", fps);
        ImGui::DragFloat2("Quad Size", (float*)(&tile_size), 1, 1, 64);
    ImGui::End();
}