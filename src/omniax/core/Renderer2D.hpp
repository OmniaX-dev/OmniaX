#ifndef __RENDERER_2D_HPP__
#define __RENDERER_2D_HPP__

#include <omniax/utils/Geometry.hpp>
#include <omniax/graphics/Color.hpp>
#include <omniax/graphics/Texture.hpp>

namespace ox
{
	class VertexArray;
	class Shader;
	class Renderer2D
	{
		public: struct tRenderStats
		{
			uint32_t drawCalls { 0 };
			uint32_t quadCount { 0 };
		};

		public:
			static void init(void);
			static void shutdown(void);

			static void beginBatch(void);
			static void endBatch(void);
			static void flush(void);

			static const tRenderStats& getRenderStats(void);
			static void resetStats(void);

			static void drawQuad(const Vec2& pos, int32_t drawLayer, const Vec2& size, const Color& color);
			static void drawQuad(const Vec2& pos, int32_t drawLayer, const Vec2& size, ResourceID texture, TextureAtlasIndex tile_index = Texture::FullTextureCoords);

			static void drawLine(const Vec2& start, const Vec2& end, int32_t drawLayer, float thickness, const Color& color);

			static Shader& bindShader(ResourceID shader);

			static void drawSingle(const VertexArray& vao, const Shader& shader);
	};
}

#endif