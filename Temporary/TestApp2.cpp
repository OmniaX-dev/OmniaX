#include "TestApp.hpp"

void Ray::draw(void)
{
	ox::Renderer2D::drawLine(position, direction, 1, 1, { 255, 0, 0 });
}

ox::Vec2 Ray::cast(Edge edge)
{
	float x1 = edge.start.x;
	float y1 = edge.start.y;
	float x2 = edge.end.x;
	float y2 = edge.end.y;

	float x3 = position.x;
	float y3 = position.y;
	float x4 = (position + direction).x;
	float y4 = (position + direction).y;

	float den = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
	if (den == 0) return position;

	float t = ((x1 - x3) * (y3 - y4) - (y1 - y3) * (x3 - x4)) / den;
	float u = -((x1 - x2) * (y1 - y3) - (y1 - y2) * (x1 - x3)) / den;

	if (t > 0 && t < 1 && u > 0)
	{
		ox::Vec2 pt;
		pt.x = x1 + t * (x2 - x1);
		pt.y = y1 + t * (y2 - y1);
		return pt;
	}
	return position;
}




void TestApp::onSetup(void)
{
	camera.create(0.0f, m_windowWidth * 1.0f, m_windowHeight * 1.0f, 0.0f);
	model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	defaultShader = ox::ResourceManager::getDefaultShader();
	ox::ResourceManager::getShader(defaultShader).bind();
	int32_t samplers[16];
	for (uint8_t i = 0; i < 16; i++)
		samplers[i] = i;
	glUniform1iv(ox::ResourceManager::getShader(defaultShader).gl_getUniformLocation("u_textures"), 16, samplers); //TODO: Add functions for uniform vectors to ox::Shader
	addOpenGLClearBit(GL_DEPTH_BUFFER_BIT);

	for (uint32_t i = 0; i < worldWidth * worldHeight; i++)
		world.push_back(Cell());

	shadowTexture.create(m_windowWidth, m_windowHeight);

	ray.position = { 200, 200 };
	world[12 * worldWidth + 10].exists = true;
}

void TestApp::onFrameStart(void) {  }
void TestApp::onFrameEnd(void) {  }
void TestApp::onUpdate(void)
{
	createEdgeList(0, 0, worldWidth, worldHeight, tile_size.x, worldWidth);
	ray.updateDirection(mousePos);
}

void TestApp::onRender(void)
{
	auto& shader = ox::Renderer2D::bindShader(defaultShader);
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

	ox::Renderer2D::drawQuad(ray.position - 3, 1, { 6, 6 }, { 200, 0, 200 });
	ray.draw();
	if (edges.size() > 0)
	{
		auto pt = ray.cast(edges[2]);
		std::cout << pt << "\n";
		if (pt != ray.position)
			ox::Renderer2D::drawQuad(pt - 3, 1, { 6, 6 }, { 200, 200, 0 });
	}

	ox::Renderer2D::endBatch();
	ox::Renderer2D::flush();
	// ox::Renderer2D::setRenderTarget(shadowTexture);
	// ox::Renderer2D::enableDepthTest();
	// ox::Renderer2D::clear({ 0, 0, 0, 0 }, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// if (vecVisibilityPolygonPoints.size() > 1)
	// {
	// 	for (int i = 0; i < vecVisibilityPolygonPoints.size() - 1; i++)
	// 	{
	// 		ox::Renderer2D::drawTriangle({ mousePos,
	// 									{ std::get<1>(vecVisibilityPolygonPoints[i]), std::get<2>(vecVisibilityPolygonPoints[i]) },
	// 									{ std::get<1>(vecVisibilityPolygonPoints[i + 1]), std::get<2>(vecVisibilityPolygonPoints[i + 1]) } },
	// 									1, { 255, 255, 255 });

	// 	}
	// 	ox::Renderer2D::drawTriangle({ mousePos,
	// 								{ std::get<1>(vecVisibilityPolygonPoints[vecVisibilityPolygonPoints.size() - 1]), std::get<2>(vecVisibilityPolygonPoints[vecVisibilityPolygonPoints.size() - 1]) },
	// 								{ std::get<1>(vecVisibilityPolygonPoints[0]), std::get<2>(vecVisibilityPolygonPoints[0]) } },
	// 								1, { 255, 255, 255 });
	// }
	// ox::Renderer2D::drawTriangle({ { 100, 100 }, { 200, 200 }, { 100, 200 } }, 1, { 128, 20, 150 });
	// ox::Renderer2D::endBatch();
	// ox::Renderer2D::flush();

	// ox::Renderer2D::setDefaultRenderTarget();
	// ox::Renderer2D::beginBatch();
	// ox::Renderer2D::clear({ 0, 0, 0 }, GL_COLOR_BUFFER_BIT);
	// ox::Renderer2D::enableDepthTest(false);

	// ox::Renderer2D::drawQuad({ 0.0f, 0.0f }, 0, { (float)m_windowWidth, (float)m_windowHeight }, shadowTexture.getResourceID());
	// ox::Renderer2D::endBatch();
	// ox::Renderer2D::flush();
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
	mousePos = { (float)evt.x, (float)evt.y };
}

void TestApp::calculateVisibilityPolygon(float ox, float oy, float radius)
{
	// Get rid of existing polygon
	vecVisibilityPolygonPoints.clear();

	// For each edge in PolyMap
	for (auto &e1 : edges)
	{
		// Take the start point, then the end point (we could use a pool of
		// non-duplicated points here, it would be more optimal)
		for (int i = 0; i < 2; i++)
		{
			float rdx, rdy;
			rdx = (i == 0 ? e1.start.x : e1.end.x) - ox;
			rdy = (i == 0 ? e1.start.y : e1.end.y) - oy;

			float base_ang = atan2f(rdy, rdx);

			float ang = 0;
			// For each point, cast 3 rays, 1 directly at point
			// and 1 a little bit either side
			for (int j = 0; j < 3; j++)
			{
				if (j == 0)	ang = base_ang - 0.0001f;
				if (j == 1)	ang = base_ang;
				if (j == 2)	ang = base_ang + 0.0001f;

				// Create ray along angle for required distance
				rdx = radius * cosf(ang);
				rdy = radius * sinf(ang);

				float min_t1 = INFINITY;
				float min_px = 0, min_py = 0, min_ang = 0;
				bool bValid = false;

				// Check for ray intersection with all edges
				for (auto &e2 : edges)
				{
					// Create line segment vector
					float sdx = e2.end.x - e2.start.x;
					float sdy = e2.end.y - e2.start.y;

					if (fabs(sdx - rdx) > 0.0f && fabs(sdy - rdy) > 0.0f)
					{
						// t2 is normalised distance from line segment start to line segment end of intersect point
						float t2 = (rdx * (e2.start.y - oy) + (rdy * (ox - e2.start.x))) / (sdx * rdy - sdy * rdx);
						// t1 is normalised distance from source along ray to ray length of intersect point
						float t1 = (e2.start.x + sdx * t2 - ox) / rdx;

						// If intersect point exists along ray, and along line 
						// segment then intersect point is valid
						if (t1 > 0 && t2 >= 0 && t2 <= 1.0f)
						{
							// Check if this intersect point is closest to source. If
							// it is, then store this point and reject others
							if (t1 < min_t1)
							{
								min_t1 = t1;
								min_px = ox + rdx * t1;
								min_py = oy + rdy * t1;
								min_ang = atan2f(min_py - oy, min_px - ox);
								bValid = true;
							}
						}
					}
				}

				if(bValid)// Add intersection point to visibility polygon perimeter
				{
					vecVisibilityPolygonPoints.push_back({ min_ang, min_px, min_py });
					std::cout << min_ang << "\n";
				}
			}
		}
		std::cout << "\n\n\n";
	}

	// Sort perimeter points by angle from source. This will allow
	// us to draw a triangle fan.
	std::sort(
		vecVisibilityPolygonPoints.begin(),
		vecVisibilityPolygonPoints.end(),
		[&](const std::tuple<float, float, float> &t1, const std::tuple<float, float, float> &t2)
		{
			return std::get<0>(t1) < std::get<0>(t2);
		});

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