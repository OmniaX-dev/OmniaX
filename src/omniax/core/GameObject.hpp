#ifndef __GAME_OBJECT_HPP__
#define __GAME_OBJECT_HPP__

#include <omniax/utils/BaseObject.hpp>
#include <omniax/core/DataStructures.hpp>
#include <omniax/utils/Geometry.hpp>
#include <omniax/graphics/Color.hpp>
#include <omniax/core/ResourceManager.hpp>

namespace ox
{
	class RenderTarget;
	class GameObject : public BaseObject
	{
		public:
			GameObject(Rectangle rect = { 0.0f, 0.0f, 16.0f, 16.0f });
			inline virtual ~GameObject(void) {  }
			
			GameObject& rotate(float angle);
			GameObject& translate(Vec2 translation);
			GameObject& scale(Vec2 scale);

			inline virtual void draw(const RenderTarget& target) {  }
			inline virtual void update(const Vec2& force = { 0.0f, 0.0f }) {  }

			//TODO: Make protected
			void render(const RenderTarget& target, ResourceID texture = ResourceManager::InvalidResource, TextureAtlasIndex tile_index = Texture::FullTextureCoords);

			inline Color getTintColor(void) const { return m_tintColor; }
			inline void setTintColor(Color color) { m_tintColor = color; }
			inline const Transform2D getTransform(void) const { return m_transform; }
			inline Rectangle getBounds(void) const { return m_bounds; }
			inline Rectangle getBaseRect(void) const { return m_rect; }
			inline void originCentered(bool b = true) { m_transform.centeredOrigin = b; }
			inline bool isOriginCentered(void) { return m_transform.centeredOrigin; }
			inline bool isVisible(void) { return m_visible; }
			inline void setBaseRect(Rectangle r) { m_rect = r; update_transform(); }

		protected:
			void update_transform(void);
			inline virtual void setVisible(bool v) { m_visible = v; }

		private:
			std::vector<Vec2> m_vertices;
			Color m_tintColor;
			Transform2D m_transform;
			Rectangle m_rect;
			Rectangle m_bounds;
			bool m_visible;
	};
}

#endif