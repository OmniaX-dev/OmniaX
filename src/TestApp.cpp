#include "TestApp.hpp"

void TestApp::onSetup(void)
{
	camera.create(0.0f, m_windowWidth * 1.0f, m_windowHeight * 1.0f, 0.0f);
	model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	baseShader = ox::ResourceManager::loadShader("baseShader3");
	addOpenGLClearBit(GL_DEPTH_BUFFER_BIT);

	for (uint32_t i = 0; i < worldWidth * worldHeight; i++)
		world.push_back(Cell());
}

void TestApp::onFrameStart(void) {  }
void TestApp::onFrameEnd(void) {  }
void TestApp::onUpdate(void)
{
	createEdgeList(0, 0, worldWidth, worldHeight, tile_size.x, worldWidth);
}

void TestApp::onRender(void)
{
	auto& shader = ox::Renderer2D::bindShader(baseShader);
	shader.updateUniform_mat4f("u_viewProjMatrix", camera.getViewProjectionMatrix());
	shader.updateUniform_mat4f("u_modelMatrix", model);

	bool test = false;
	ox::Renderer2D::beginBatch();
	for (size_t y = 0; y < worldHeight; y++)
	{
		for (size_t x = 0; x < worldWidth; x++)
		{
			if (world[y * worldWidth + x].exists)
				ox::Renderer2D::drawQuad({ x * tile_size.x, y * tile_size.y }, 0, { tile_size.x, tile_size.y }, { 0, 128, 0 }); 
		}
	}
	
	for (auto &e : edges)
	{
		ox::Renderer2D::drawLine(e.start, e.end, 0, 3, { 200, 200, 200 });
		ox::Renderer2D::drawQuad({ e.start - 2.0f }, 1, { 4.0f, 4.0f }, {255, 0, 0});
		ox::Renderer2D::drawQuad({ e.end - 2.0f }, 1, { 4.0f, 4.0f }, {255, 0, 0});
	}
	

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
	if (evt.button == ox::tMouseButtons::Left)
	{
		uint32_t i = ((int)evt.y / (int)tile_size.y) * worldWidth + ((int)evt.x / (int)tile_size.x);
		world[i].exists = !world[i].exists;
	}
}

void TestApp::onMouseMoved(const ox::MouseMovedEvent& evt)
{
}

void TestApp::createEdgeList(int32_t sx, int32_t sy, int32_t w, int32_t h, float blockWidth, int32_t pitch)
{
		edges.clear();

		for (int x = 0; x < w; x++)
		{
			for (int y = 0; y < h; y++)
			{
				for (int j = 0; j < 4; j++)
				{
					world[(y + sy) * pitch + (x + sx)].has_edge[j] = false;
					world[(y + sy) * pitch + (x + sx)].edge_index[j] = 0;
				}
			}
		}

		// Iterate through region from top left to bottom right
		for (int x = 1; x < w - 1; x++)
		{
			for (int y = 1; y < h - 1; y++)
			{
				// Create some convenient indices
				int i = (y + sy) * pitch + (x + sx);			// This
				int n = (y + sy - 1) * pitch + (x + sx);		// Northern Neighbour
				int s = (y + sy + 1) * pitch + (x + sx);		// Southern Neighbour
				int w = (y + sy) * pitch + (x + sx - 1);	// Western Neighbour
				int e = (y + sy) * pitch + (x + sx + 1);	// Eastern Neighbour

				// If this cell exists, check if it needs edges
				if (world[i].exists)
				{
					// If this cell has no western neighbour, it needs a western edge
					if (!world[w].exists)
					{
						// It can either extend it from its northern neighbour if they have
						// one, or It can start a new one.
						if (world[n].has_edge[WEST])
						{
							// Northern neighbour has a western edge, so grow it downwards
							edges[world[n].edge_index[WEST]].end.y += blockWidth;
							world[i].edge_index[WEST] = world[n].edge_index[WEST];
							world[i].has_edge[WEST] = true;
						}
						else
						{
							// Northern neighbour does not have one, so create one
							Edge edge;
							edge.start.x = (sx + x) * blockWidth; edge.start.y = (sy + y) * blockWidth;
							edge.end.x = edge.start.x; edge.end.y = edge.start.y + blockWidth;

							// Add edge to Polygon Pool
							int edge_index = edges.size();
							edges.push_back(edge);

							// Update tile information with edge information
							world[i].edge_index[WEST] = edge_index;
							world[i].has_edge[WEST] = true;
						}
					}

					// If this cell dont have an eastern neignbour, It needs a eastern edge
					if (!world[e].exists)
					{
						// It can either extend it from its northern neighbour if they have
						// one, or It can start a new one.
						if (world[n].has_edge[EAST])
						{
							// Northern neighbour has one, so grow it downwards
							edges[world[n].edge_index[EAST]].end.y += blockWidth;
							world[i].edge_index[EAST] = world[n].edge_index[EAST];
							world[i].has_edge[EAST] = true;
						}
						else
						{
							// Northern neighbour does not have one, so create one
							Edge edge;
							edge.start.x = (sx + x + 1) * blockWidth; edge.start.y = (sy + y) * blockWidth;
							edge.end.x = edge.start.x; edge.end.y = edge.start.y + blockWidth;

							// Add edge to Polygon Pool
							int edge_index = edges.size();
							edges.push_back(edge);

							// Update tile information with edge information
							world[i].edge_index[EAST] = edge_index;
							world[i].has_edge[EAST] = true;
						}
					}

					// If this cell doesnt have a northern neignbour, It needs a northern edge
					if (!world[n].exists)
					{
						// It can either extend it from its western neighbour if they have
						// one, or It can start a new one.
						if (world[w].has_edge[NORTH])
						{
							// Western neighbour has one, so grow it eastwards
							edges[world[w].edge_index[NORTH]].end.x += blockWidth;
							world[i].edge_index[NORTH] = world[w].edge_index[NORTH];
							world[i].has_edge[NORTH] = true;
						}
						else
						{
							// Western neighbour does not have one, so create one
							Edge edge;
							edge.start.x = (sx + x) * blockWidth; edge.start.y = (sy + y) * blockWidth;
							edge.end.x = edge.start.x + blockWidth; edge.end.y = edge.start.y;

							// Add edge to Polygon Pool
							int edge_index = edges.size();
							edges.push_back(edge);

							// Update tile information with edge information
							world[i].edge_index[NORTH] = edge_index;
							world[i].has_edge[NORTH] = true;
						}
					}

					// If this cell doesnt have a southern neignbour, It needs a southern edge
					if (!world[s].exists)
					{
						// It can either extend it from its western neighbour if they have
						// one, or It can start a new one.
						if (world[w].has_edge[SOUTH])
						{
							// Western neighbour has one, so grow it eastwards
							edges[world[w].edge_index[SOUTH]].end.x += blockWidth;
							world[i].edge_index[SOUTH] = world[w].edge_index[SOUTH];
							world[i].has_edge[SOUTH] = true;
						}
						else
						{
							// Western neighbour does not have one, so I need to create one
							Edge edge;
							edge.start.x = (sx + x) * blockWidth; edge.start.y = (sy + y + 1) * blockWidth;
							edge.end.x = edge.start.x + blockWidth; edge.end.y = edge.start.y;

							// Add edge to Polygon Pool
							int edge_index = edges.size();
							edges.push_back(edge);

							// Update tile information with edge information
							world[i].edge_index[SOUTH] = edge_index;
							world[i].has_edge[SOUTH] = true;
						}
					}
				}
			}
		}
	}