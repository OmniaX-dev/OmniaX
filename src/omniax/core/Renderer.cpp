#include "Renderer.hpp"
#include "GLBuffers.hpp"
#include "Shader.hpp"
#include "Errors.hpp"
#include "DataStructures.hpp"
#include <cstring>

namespace ox
{
	static constexpr size_t MaxQuadCount = 1024;
	static constexpr size_t MaxVertexCount = MaxQuadCount * 4;
	static constexpr size_t MaxIndexCount = MaxQuadCount * 6;
	static constexpr size_t MaxTextures = 16;

	struct tRendererData
	{
		VertexArray vao { false };
		VertexBuffer vbo;
		ElementBuffer ebo;

		uint32_t whiteTexture { 0 };
		int32_t whiteTextureSlot { 0 };

		uint32_t indexCount { 0 };

		tVertex* buffer { nullptr };
		tVertex* bufferPtr { nullptr };

		uint32_t texSlots[MaxTextures];
		uint32_t texSlotIndex { 0 };

		Renderer::tRenderStats renderStats;
	};

	static tRendererData s_rendererData;

	void Renderer::init(void)
	{
		s_rendererData.buffer = new tVertex[MaxVertexCount];
		s_rendererData.vao.create();
		s_rendererData.vao.bind();

		s_rendererData.vbo.create(nullptr, MaxVertexCount * sizeof(tVertex), true);
		s_rendererData.vao.addBuffer(s_rendererData.vbo, tVertex::getVertexBufferLayout());

		int32_t indices[MaxIndexCount];
		uint32_t offset = 0;
		for (size_t i = 0; i < MaxIndexCount; i += 6)
		{
			indices[i + 0] = 0 + offset;
			indices[i + 1] = 1 + offset;
			indices[i + 2] = 2 + offset;
			
			indices[i + 3] = 2 + offset;
			indices[i + 4] = 3 + offset;
			indices[i + 5] = 0 + offset;

			offset += 4;
		}

		s_rendererData.ebo.create(indices, MaxIndexCount);
		s_rendererData.vao.setElementBuffer(s_rendererData.ebo);

		GLCall(glGenTextures(1, &s_rendererData.whiteTexture));
		GLCall(glBindTexture(GL_TEXTURE_2D, s_rendererData.whiteTexture));

		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
		uint32_t color = 0xffffffff;
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, &color);

		s_rendererData.texSlots[0] = s_rendererData.whiteTexture;
		for (size_t i = 1; i < MaxTextures; i++)
			s_rendererData.texSlots[i] = 0;
	}

	void Renderer::shutdown(void)
	{
		GLCall(glDeleteTextures(1, &s_rendererData.whiteTexture));
		delete[] s_rendererData.buffer;
	}
	
	void Renderer::beginBatch(void)
	{
		s_rendererData.bufferPtr = s_rendererData.buffer;
	}
	
	void Renderer::endBatch(void)
	{
		GLsizeiptr size = (uint8_t*)s_rendererData.bufferPtr - (uint8_t*)s_rendererData.buffer;
		s_rendererData.vbo.bind();
		GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, size, s_rendererData.buffer));
	}
	
	void Renderer::flush(void)
	{
		for (uint32_t i = 0; i < s_rendererData.texSlotIndex; i++)
		{
			GLCall(glActiveTexture(GL_TEXTURE0 + i));
			GLCall(glBindTexture(GL_TEXTURE_2D, s_rendererData.texSlots[i]));
		}
		s_rendererData.vao.bind();
		GLCall(glDrawElements(GL_TRIANGLES, s_rendererData.indexCount, GL_UNSIGNED_INT, nullptr));
		s_rendererData.renderStats.drawCalls++;

		s_rendererData.indexCount = 0;
		s_rendererData.texSlotIndex = 1;
	}
	
	const Renderer::tRenderStats& Renderer::getRenderStats(void)
	{
		return s_rendererData.renderStats;
	}
	
	void Renderer::resetStats(void)
	{
		memset(&s_rendererData.renderStats, 0, sizeof(Renderer::tRenderStats));
	}

	void Renderer::drawQuad(const Vec3& pos, const Vec2& size, const Color& color)
	{
		if (s_rendererData.indexCount >= MaxIndexCount)
		{
			Renderer::endBatch();
			Renderer::flush();
			Renderer::beginBatch();
		}

		float texIndex = 0.0f;

		s_rendererData.bufferPtr->color = color.getNormalizedColor();
		s_rendererData.bufferPtr->texCoords = { 0.0f, 1.0f };
		s_rendererData.bufferPtr->texIndex = texIndex;
		s_rendererData.bufferPtr->position = pos;
		s_rendererData.bufferPtr++;

		s_rendererData.bufferPtr->color = color.getNormalizedColor();
		s_rendererData.bufferPtr->texCoords = { 1.0f, 1.0f };
		s_rendererData.bufferPtr->texIndex = texIndex;
		s_rendererData.bufferPtr->position = { pos.x + size.x, pos.y, pos.z };
		s_rendererData.bufferPtr++;

		s_rendererData.bufferPtr->color = color.getNormalizedColor();
		s_rendererData.bufferPtr->texCoords = { 1.0f, 0.0f };
		s_rendererData.bufferPtr->texIndex = texIndex;
		s_rendererData.bufferPtr->position = { pos.x + size.x, pos.y + size.y, pos.z };
		s_rendererData.bufferPtr++;

		s_rendererData.bufferPtr->color = color.getNormalizedColor();
		s_rendererData.bufferPtr->texCoords = { 0.0f, 0.0f };
		s_rendererData.bufferPtr->texIndex = texIndex;
		s_rendererData.bufferPtr->position = { pos.x, pos.y + size.y, pos.z };
		s_rendererData.bufferPtr++;

		s_rendererData.indexCount += 6;
		s_rendererData.renderStats.quadCount++;
	}

    void Renderer::drawQuad(const Vec3& pos, const Vec2& size, const Texture& texture)
	{
		if (s_rendererData.indexCount >= MaxIndexCount || s_rendererData.texSlotIndex >= MaxTextures)
		{
			Renderer::endBatch();
			Renderer::flush();
			Renderer::beginBatch();
		}

		float texIndex = 0.0f;
		for (uint32_t i = 1; i < s_rendererData.texSlotIndex; i++)
		{
			if (s_rendererData.texSlots[i] == texture.getOpenGLID())
			{
				texIndex = (float)i;
				break;
			}
		}

		if (texIndex == 0.0f)
		{
			texIndex = (float)s_rendererData.texSlotIndex;
			s_rendererData.texSlots[s_rendererData.texSlotIndex] = texture.getOpenGLID();
			s_rendererData.texSlotIndex++;
		}
		//std::cout <<  << "\n";
		
		Color color { 255 };

		s_rendererData.bufferPtr->color = color.getNormalizedColor();
		s_rendererData.bufferPtr->texCoords = { 0.0f, 1.0f };
		s_rendererData.bufferPtr->texIndex = texIndex;
		s_rendererData.bufferPtr->position = pos;
		s_rendererData.bufferPtr++;

		s_rendererData.bufferPtr->color = color.getNormalizedColor();
		s_rendererData.bufferPtr->texCoords = { 1.0f, 1.0f };
		s_rendererData.bufferPtr->texIndex = texIndex;
		s_rendererData.bufferPtr->position = { pos.x + size.x, pos.y, pos.z };
		s_rendererData.bufferPtr++;

		s_rendererData.bufferPtr->color = color.getNormalizedColor();
		s_rendererData.bufferPtr->texCoords = { 1.0f, 0.0f };
		s_rendererData.bufferPtr->texIndex = texIndex;
		s_rendererData.bufferPtr->position = { pos.x + size.x, pos.y + size.y, pos.z };
		s_rendererData.bufferPtr++;

		s_rendererData.bufferPtr->color = color.getNormalizedColor();
		s_rendererData.bufferPtr->texCoords = { 0.0f, 0.0f };
		s_rendererData.bufferPtr->texIndex = texIndex;
		s_rendererData.bufferPtr->position = { pos.x, pos.y + size.y, pos.z };
		s_rendererData.bufferPtr++;

		s_rendererData.indexCount += 6;
		s_rendererData.renderStats.quadCount++;
	}
	
	

	void Renderer::drawSingle(const VertexArray& vao, const Shader& shader)
	{
		shader.bind();
		vao.bind();
		GLCall(glDrawElements(GL_TRIANGLES, vao.getElementCount(), GL_UNSIGNED_INT, 0));
		// GLCall(glDrawElements(GL_TRIANGLES, vao.getElementCount(), GL_UNSIGNED_INT, 0));
	}
}