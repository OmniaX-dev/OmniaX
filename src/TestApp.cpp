#include "TestApp.hpp"


void TestApp::onSetup(void)
{
	camera.create(0.0f, m_windowWidth * 1.0f, m_windowHeight * 1.0f, 0.0f);
	defaultShader = ox::ResourceManager::loadShader("baseShader2");
	ox::ResourceManager::getShader(defaultShader).bind();
	int32_t samplers[16];
	for (uint8_t i = 0; i < 16; i++)
		samplers[i] = i;
	glUniform1iv(ox::ResourceManager::getShader(defaultShader).gl_getUniformLocation("u_textures"), 16, samplers); //TODO: Add functions for uniform vectors to ox::Shader
	addOpenGLClearBit(GL_DEPTH_BUFFER_BIT);

	skyTex = ox::ResourceManager::loadTexture("res/sky.png");
	playerTex = ox::ResourceManager::loadTexture("res/player.png");
	auto& tex = ox::ResourceManager::getTexture(playerTex);
	anim.create("TestAnim", playerTex, {}, 3);
	anim.addFrame(tex.addTileInfo(0, 0, 32, 32));
	anim.addFrame(tex.addTileInfo(32, 0, 32, 32));
	anim.addFrame(tex.addTileInfo(64, 0, 32, 32));

	object.translate({ 200, 200 });
	object.scale({ 4, 4 });
	object.originCentered(true);

	notoBF = ox::ResourceManager::loadBitmapFont("res/fonts/noto.png");

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
	snow = ox::ParticleFactory::basicSnowEmitter(partTex, { (float)m_windowWidth, (float)m_windowHeight - (192 - 65) });
	snow.getDefaultParticleInfo().speed = 0.75f;
	snow.getDefaultParticleInfo().tileIndex = partTiles[0];
	snow.getDefaultParticleInfo().color = { 132, 165, 216 };
	snow.getDefaultParticleInfo().damping = { 0.08f, 0.0f };
	snow.getDefaultParticleInfo().randomDamping = true;
	snow.setMaxParticleCount(2000);
	fire = ox::ParticleFactory::basicFireEmitter(partTex, { 768 + 28, (float)(m_windowHeight - 150) });
	fire.getDefaultParticleInfo().tileIndex = partTiles[0];
	// fire.getDefaultParticleInfo().lifeSpan = 500;
	fire.setMaxParticleCount(1000);
	fire.useTileArray(true);
	fire.addTilesToArray(partTiles);

	srand(time(nullptr));

	uint32_t npoints = 15;
	float radius = 200.0f;
	for (int i = 0; i < npoints; i++)
	{
		ox::Vec2 rnd { ox::Utils::get_rand_float(-300.0f, 300.0f), ox::Utils::get_rand_float(-100.0f, 100.0f) };
		fire.addPathPoint({ radius * std::sin((float)i / (float)npoints * PI * 2.0f) + m_windowWidth / 2 + rnd.x,
							radius * std::cos((float)i / (float)npoints * PI * 2.0f) + m_windowHeight / 2 + rnd.y });
	}

	fire.enableEditablePath();
	fire.enablePath(false);

	ox::Renderer2D::Text::font = notoBF;
	ox::Renderer2D::Text::characterHeight = 48;
}

void TestApp::onFrameStart(void) {  }
void TestApp::onFrameEnd(void) {  }
void TestApp::onUpdate(void)
{
	object.rotate(1);
	ox::Vec2 wind { 0.0f, 0.0f };
	fire.emit(RANDOM(1, 3));
	snow.emit(RANDOM(1, 2));
	if (current++ > 30)
	{
		current = 0;
		wind.x = ox::Utils::get_rand_float(0.002f, 0.09f);
	}
	snow.update();
	fire.update(wind);
	snow.update(wind);
}

void TestApp::onRender(void)
{
	auto& shader = ox::Renderer2D::bindShader(defaultShader);
	shader.updateUniform_mat4f("u_viewProjMatrix", camera.getViewProjectionMatrix());


	ox::Renderer2D::beginBatch();
	ox::Renderer2D::drawQuad(ox::Renderer2D::getStaticQuad({ 0, -80 }, { 1024, 540 }, false), { 255 }, skyTex);
	snow.draw(ox::Renderer2D::getCurrentRenderTarget());
	ox::Renderer2D::drawQuad(ox::Renderer2D::getStaticQuad({ 0, (float)(m_windowHeight - 192) }, { 256, 192 }, false), { 170, 150, 230 }, terrainTex, snowTile);
	ox::Renderer2D::drawQuad(ox::Renderer2D::getStaticQuad({ 256, (float)(m_windowHeight - 192) }, { 256, 192 }, false), { 170, 150, 230 }, terrainTex, snowTile);
	ox::Renderer2D::drawQuad(ox::Renderer2D::getStaticQuad({ 512, (float)(m_windowHeight - 192) }, { 256, 192 }, false), { 170, 150, 230 }, terrainTex, snowTile);
	ox::Renderer2D::drawQuad(ox::Renderer2D::getStaticQuad({ 768, (float)(m_windowHeight - 192) }, { 256, 192 }, false), { 170, 150, 230 }, terrainTex, snowTile);
	ox::Renderer2D::drawQuad(ox::Renderer2D::getStaticQuad({ 768, (float)(m_windowHeight - 192 + 10) }, { 64, 64 }, false), { 255 }, terrainTex, logsTile);
	fire.draw(ox::Renderer2D::getCurrentRenderTarget());

	ox::Renderer2D::drawQuad({ 50.0f, 100.0f }, ox::Renderer2D::Text::getStringBounds("Hello World!!", { 5.0f, 5.0f }), { 255, 255, 255 } );
	ox::Renderer2D::tTextInfo info { 22.0f, 10.0f, notoBF, { 50, 255, 20 } };
	ox::Renderer2D::Text::drawCentered("Ciao Mondo!!", { m_windowWidth / 2.0f, m_windowHeight / 2.0f }, info);
	ox::Renderer2D::Text::draw("Hello World!!", { 55.0f, 105.0f }, ox::Color { 200, 50, 20 });

	ox::Renderer2D::endBatch();
	ox::Renderer2D::flush();
}


void TestApp::onImGuiRender(void)
{
	ImGui::Begin("Controls");
		ImGui::LabelText("", "DrawCalls: %d", ox::Renderer2D::getRenderStats().drawCalls);
		ImGui::LabelText("", "Quads: %d", ox::Renderer2D::getRenderStats().quadCount);
		ImGui::LabelText("", "FPS: %d", getFps());
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
