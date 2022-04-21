#ifndef __RENDERER_HPP__
#define __RENDERER_HPP__

#include <omniax/utils/Geometry.hpp>
#include <omniax/graphics/Color.hpp>
#include <omniax/graphics/Texture.hpp>

namespace ox
{
	class VertexArray;
	class Shader;
	class Renderer
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

			static void drawQuad(const Vec3& pos, const Vec2& size, const Color& color);
			static void drawQuad(const Vec3& pos, const Vec2& size, const Texture& texture);

			static void drawSingle(const VertexArray& vao, const Shader& shader);
	};
}

#endif