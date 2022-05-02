#include "TestApp.hpp"
//#include <omniax/vendor/imgui/imgui_demo.cpp>

void TestObj::handleSignal(ox::tSignal& signal)
{
    std::cout << signal.userData.getTypeName() << ": Signal ID: " << (int)(signal.ID) << "\n";
    signal.handled = true;
}

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

    setCustomData1(camera);

    obj.connectSignal(ox::tBuiltinSignals::MouseMoved);
    obj.connectSignal(ox::tBuiltinSignals::MouseReleased);
    obj.connectSignal(ox::tBuiltinSignals::KeyPressed);

    fic.add(colInterp1);
    fic.add(colInterp2);
    fic.add(colInterp3);
}

void TestApp::onFrameStart(void) {  }
void TestApp::onFrameEnd(void) {  }
void TestApp::onUpdate(void) {  }

void TestApp::onRender(void)
{
    fic.update();
    baseShader.bind();
    baseShader.updateUniform_mat4f("u_viewProjMatrix", camera.getViewProjectionMatrix());
    baseShader.updateUniform_mat4f("u_modelMatrix", model);
    baseShader.updateUniform_vec4f("u_testColor", fic.get());

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


void TestApp::onImGuiRender(void)
{
    //ImGui::ShowDemoWindow();
    ImGui::Begin("Controls");
        ImGui::LabelText("", "DrawCalls: %d", ox::Renderer::getRenderStats().drawCalls);
        ImGui::LabelText("", "Quads: %d", ox::Renderer::getRenderStats().quadCount);
        ImGui::LabelText("", "FPS: %d", getFps());
        ImGui::DragFloat2("Quad Size", (float*)(&tile_size), 1, 1, 64);
    ImGui::End();
}

void TestApp::onKeyPressed(const ox::KeyEvent& evt)
{
    OX_INFO("Key Pressed %d", evt.key);
}

void TestApp::onKeyReleased(const ox::KeyEvent& evt)
{
    OX_INFO("Key Released %d", evt.key);
}

void TestApp::onMousePressed(const ox::MouseButtonEvent& evt)
{
    if (evt.button == ox::tMouseButtons::Left)
        OX_INFO("Mouse Pressed X: %d, Y: %d", evt.x, evt.y);
    else if (evt.button == ox::tMouseButtons::Right)
        OX_INFO("%s", evt.app.getCustomData1().getTypeName().c_str());
}

void TestApp::onMouseReleased(const ox::MouseButtonEvent& evt)
{
    OX_INFO("Mouse Released X: %d, Y: %d", evt.x, evt.y);
}

void TestApp::onMouseMoved(const ox::MouseMovedEvent& evt)
{
    OX_INFO("Mouse Moved: %d, %d", evt.x, evt.y);
}