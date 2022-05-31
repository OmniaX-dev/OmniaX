#ifndef __RENDERER_2D_HPP__
#define __RENDERER_2D_HPP__

#include <omniax/utils/Geometry.hpp>
#include <omniax/graphics/Color.hpp>
#include <omniax/graphics/Texture.hpp>
#include <omniax/core/DataStructures.hpp>
#include <omniax/core/ResourceManager.hpp>

namespace ox
{
	class VertexArray;
	class Shader;
	class RenderTarget;
	class Renderer2D
	{
		public: struct tRenderStats
		{
			uint32_t drawCalls { 0 };
			uint32_t quadCount { 0 };
		};

		public: struct tTextInfo
		{
			float characterHeight { 32.0f };
			float characterSpacing { 5.0f };
			ResourceID font { ResourceManager::InvalidResource };
			Color color { 255, 255, 255 };
		};

		public: class Text
		{
			public:
				static void draw(const String& text, const Vec2& position, const Color& color);
				static void draw(const String& text, const Vec2& position, const tTextInfo& info);
				static void drawCentered(const String& text, const Vec2& position, const Color& color);
				static void drawCentered(const String& text, const Vec2& position, const tTextInfo& info);
				static Vec2 getStringBounds(const String& text, Vec2 padding = { 0.0f, 0.0f }, float characterHeight = 0.0f, float spacing = 0.0f);
				inline static void setFont(ResourceID _font) { font = _font; }

			public:
				inline static ResourceID font { ResourceManager::InvalidResource };
				inline static float characterHeight { 32.0f };
				inline static float characterSpacing { 5.0f };
		};

		public:
			static void init(void);
			static void shutdown(void);

			static void beginBatch(void);
			static void endBatch(void);
			static void flush(void);

			static const tRenderStats& getRenderStats(void);
			static void resetStats(void);

			static void drawQuad(const Vec2& pos, const Vec2& size, const Color& color, Transform2D transform = Transform2D());
			static void drawQuad(const Vec2& pos, const Vec2& size, ResourceID texture, Transform2D transform = Transform2D(), TextureAtlasIndex tile_index = Texture::FullTextureCoords, const ox::Color& tint = { 255, 255, 255 });
			static void drawQuad(const std::vector<Vec2>& vertices, const Color& tintColor, ResourceID texture = ResourceManager::InvalidResource, TextureAtlasIndex tile_index = Texture::FullTextureCoords);
			static void drawLine(const Vec2& start, const Vec2& end, float thickness, const Color& color);
			static void drawTriangle(const Triangle& triangle, const Color& color);
			static void drawSingle(const VertexArray& vao, const Shader& shader);
			static void drawText(const String& text, ResourceID font, Vec2 position, Color color, float charHeight, float spacing = 5.0f);

			static Shader& bindShader(ResourceID shader);

			static void clear(const Color& color, uint32_t gl_mask);
			static void setRenderTarget(const RenderTarget& target);
			static void setDefaultRenderTarget(void);
			static void enableDepthTest(bool enable = true);

			static const RenderTarget& getCurrentRenderTarget(void);
			inline static const RenderTarget& getDefaultRenderTarget(void) { return (const RenderTarget&)BaseObject::InvalidRef(); }

			static std::vector<Vec2> getStaticQuad(Vec2 position, Vec2 size, bool centered = true);
			static std::vector<Vec2> tranformQuad(Vec2 position, Vec2 size, const Transform2D& transform);
	};

    class RenderTarget : public BaseObject
    {
        public:
            inline RenderTarget(void) { invalidate(); }
            inline RenderTarget(int32_t width, int32_t height) { create(width, height); }
            virtual ~RenderTarget(void);
            RenderTarget& create(int32_t width, int32_t height);

            inline uint32_t getOpenGLFrameBufferID(void) const { return static_cast<uint32_t>(getID()); }
            inline uint32_t getOpenGLRenderBufferID(void) const { return m_rbo_gl_id; }
			inline ResourceID getResourceID(void) { return m_texture; }

            void bind(void) const;
            void unbind(void) const;
            void bindScreenTexture(void) const;

        private:
            ResourceID m_texture;
            uint32_t m_rbo_gl_id;
            int32_t m_width;
            int32_t m_height;
    };

	typedef RenderTarget RenderTexture;
}

#endif