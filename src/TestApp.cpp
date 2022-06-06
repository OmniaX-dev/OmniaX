#include "TestApp.hpp"


void TestApp::onSetup(void)
{
	ox::Random::autoSeed();

	camera.create(0.0f, getWindowWidth() * 1.0f, getWindowHeight() * 1.0f, 0.0f);
	defaultShader = ox::ResourceManager::getDefaultShader();

	skyTex = ox::ResourceManager::loadTexture("res/sky.png");
	terrainTex = ox::ResourceManager::loadTexture("res/terrain.png");
	auto& ttex = ox::ResourceManager::getTexture(terrainTex);
	snowTile = ttex.addTileInfo(0, 0, 256, 192);
	logsTile = ttex.addTileInfo(256, 0, 64, 64);

	partTex = ox::ResourceManager::loadTexture("res/particle.png");
	auto& ptex = ox::ResourceManager::getTexture(partTex);
	partTiles.push_back(ptex.addTileInfo(0, 0, 32, 32));
	partTiles.push_back(ptex.addTileInfo(32, 0, 32, 32));
	partTiles.push_back(ptex.addTileInfo(64, 0, 32, 32));
	partTiles.push_back(ptex.addTileInfo(96, 0, 32, 32));
	partTiles.push_back(ptex.addTileInfo(128, 0, 32, 32));
	partTiles.push_back(ptex.addTileInfo(160, 0, 32, 32));
	snow = ox::ParticleFactory::basicSnowEmitter({ partTex, partTiles[0] }, { (float)getWindowWidth(), (float)getWindowHeight() - (192 - 65) }, 1000);
	fire = ox::ParticleFactory::basicFireEmitter({ partTex, partTiles[0] }, { 768 + 28, (float)(getWindowHeight() - 150) }, 1000);
	fire.useTileArray(true);
	fire.addTilesToArray(partTiles);

	scene.create(getWindowWidth(), getWindowHeight());
	gui.create(getWindowWidth(), getWindowHeight());

	ox::LightingManager::init(getWindowWidth(), getWindowHeight());
	ox::LightingManager::addLightSource({ 768 + 28, (float)(getWindowHeight() - 150) }, { 500, 500 }, { 255, 200, 110, 255 });
	ox::LightingManager::setAnbientColor({ 0, 0, 100, 150 });

	gameClock.start();
}

void TestApp::onFrameStart(void) {  }
void TestApp::onFrameEnd(void) {  }
void TestApp::onUpdate(void)
{
	ox::Vec2 wind { 0.0f, 0.0f };
	fire.emit(ox::Random::geti32(1, 3));
	snow.emit(ox::Random::geti32(1, 2));
	if (current++ > 30)
	{
		current = 0;
		wind.x = ox::Random::getf32(0.002f, 0.09f);
	}
	fire.update(wind);
	snow.update(wind);

}

void TestApp::onSecondsUpdate(void)
{
	gameClock.update();
}

void TestApp::onRender(void)
{
	auto& shader = ox::Renderer2D::bindShader(defaultShader);
	shader.updateUniform_mat4f("u_viewProjMatrix", camera.getViewProjectionMatrix());

	ox::Renderer2D::setRenderTarget(scene);
	ox::Renderer2D::clear({ 0, 0, 0, 255 });
	ox::Renderer2D::drawQuad(ox::Renderer2D::getStaticQuad({ 0, -80 }, { 1024, 540 }, false), { 255 }, skyTex);
	snow.draw();
	ox::Renderer2D::drawQuad(ox::Renderer2D::getStaticQuad({ 0, (float)(getWindowHeight() - 192) }, { 256, 192 }, false), { 170, 150, 230 }, terrainTex, snowTile);
	ox::Renderer2D::drawQuad(ox::Renderer2D::getStaticQuad({ 256, (float)(getWindowHeight() - 192) }, { 256, 192 }, false), { 170, 150, 230 }, terrainTex, snowTile);
	ox::Renderer2D::drawQuad(ox::Renderer2D::getStaticQuad({ 512, (float)(getWindowHeight() - 192) }, { 256, 192 }, false), { 170, 150, 230 }, terrainTex, snowTile);
	ox::Renderer2D::drawQuad(ox::Renderer2D::getStaticQuad({ 768, (float)(getWindowHeight() - 192) }, { 256, 192 }, false), { 170, 150, 230 }, terrainTex, snowTile);
	ox::Renderer2D::drawQuad(ox::Renderer2D::getStaticQuad({ 768, (float)(getWindowHeight() - 192 + 10) }, { 64, 64 }, false), { 255 }, terrainTex, logsTile);
	fire.draw();
	ox::LightingManager::render(camera);

	ox::Renderer2D::setRenderTarget(gui);
	ox::Renderer2D::clear({ 0, 0, 0, 0 });
	ox::Renderer2D::Text::draw(gameClock.asString(), { 5.0f, 5.0f }, ox::Color{ 255, 197, 50 });

	ox::Renderer2D::setDefaultRenderTarget();
	ox::Renderer2D::drawQuad(ox::Renderer2D::getStaticQuad({ 0, 0 }, { (float)getWindowWidth(), (float)getWindowHeight() }, false), { 255, 255, 255 }, gui.getResourceID());

	ox::Renderer2D::endBatch();
	ox::Renderer2D::flush();
}


void TestApp::onImGuiRender(void)
{
	ImGui::Begin("Controls");
		ImGui::LabelText("", "DrawCalls: %d", ox::Renderer2D::getRenderStats().drawCalls);
		ImGui::LabelText("", "Quads: %d", ox::Renderer2D::getRenderStats().quadCount);
		ImGui::LabelText("", "FPS: %d", getFps());
		ImGui::LabelText("", "Lights: %d", ox::LightingManager::getLightCount());
	ImGui::End();
}

void TestApp::onKeyPressed(const ox::KeyEvent& evt)
{
}

void TestApp::onMousePressed(const ox::MouseButtonEvent& evt)
{
}

void TestApp::onMouseReleased(const ox::MouseButtonEvent& evt)
{
}

void TestApp::onMouseMoved(const ox::MouseMovedEvent& evt)
{
}

void TestApp::onSignal(ox::tSignal& signal)
{
	if (signal.ID == ox::tBuiltinSignals::WindowResized)
		camera.create(0.0f, getWindowWidth() * 1.0f, getWindowHeight() * 1.0f, 0.0f);
}
