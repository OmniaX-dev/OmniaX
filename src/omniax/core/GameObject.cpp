#include "GameObject.hpp"
#include <omniax/core/Renderer2D.hpp>

namespace ox
{

	GameObject::GameObject(Rectangle rect)
	{
		m_rect = rect;
		m_tintColor = { 255, 255, 255, 255 };
		m_visible = true;
		update_transform();
	}

	GameObject& GameObject::rotate(float angle)
	{
		m_transform.rotation += angle;
		update_transform();
		return *this;
	}
	
	GameObject& GameObject::translate(Vec2 translation)
	{
		m_transform.translation += translation;
		update_transform();
		return *this;
	}
	
	GameObject& GameObject::scale(Vec2 scale)
	{
		m_transform.scale += scale;
		update_transform();
		return *this;
	}

	void GameObject::render(ResourceID texture, TextureAtlasIndex tile_index)
	{
		if (!m_visible) return;
		Renderer2D::drawQuad(m_vertices, m_tintColor, texture, tile_index);
	}
	
	void GameObject::update_transform(void)
	{
		m_vertices = Renderer2D::tranformQuad(m_rect.getPosition(), m_rect.getSize(), m_transform);
		float min_x = m_vertices[0].x;
		float min_y = m_vertices[0].y;
		float max_x = m_vertices[m_vertices.size() - 1].x;
		float max_y = m_vertices[m_vertices.size() - 1].y;
		for (auto& vert : m_vertices)
		{
			if (vert.x < min_x) min_x = vert.x;
			else if (vert.x > max_x) max_x = vert.x;
			if (vert.y < min_y) min_y = vert.y;
			else if (vert.y > max_y) max_y = vert.y;
		}
		m_bounds = { min_x, min_y, max_x - min_x, max_y - min_y };
	}
}