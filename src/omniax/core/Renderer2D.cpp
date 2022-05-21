#include "Renderer2D.hpp"
#include "GLBuffers.hpp"
#include "Shader.hpp"
#include "Errors.hpp"
#include "DataStructures.hpp"
#include "ResourceManager.hpp"
#include <cstring>

namespace ox
{
	static constexpr size_t MaxQuadCount = 16384;
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

		Texture* currentTex { nullptr };
		ResourceID currentTexResID { ResourceManager::InvalidResource };
		float currentTexIndex { 0.0f };
		Shader* currentShader { nullptr };
		ResourceID currentShaderResID { ResourceManager::InvalidResource };

		Renderer2D::tRenderStats renderStats;
	};

	static tRendererData s_rendererData;

	void Renderer2D::init(void)
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

		s_rendererData.currentTex = nullptr;
		s_rendererData.currentTexResID = ResourceManager::InvalidResource;
		s_rendererData.currentTexIndex = 0.0f;
		s_rendererData.currentShaderResID = ResourceManager::InvalidResource;
		s_rendererData.currentShader = nullptr;
	}

	void Renderer2D::shutdown(void)
	{
		GLCall(glDeleteTextures(1, &s_rendererData.whiteTexture));
		delete[] s_rendererData.buffer;
	}
	
	void Renderer2D::beginBatch(void)
	{
		s_rendererData.bufferPtr = s_rendererData.buffer;
	}
	
	void Renderer2D::endBatch(void)
	{
		GLsizeiptr size = (uint8_t*)s_rendererData.bufferPtr - (uint8_t*)s_rendererData.buffer;
		s_rendererData.vbo.bind();
		GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, size, s_rendererData.buffer));
	}
	
	void Renderer2D::flush(void)
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
	
	const Renderer2D::tRenderStats& Renderer2D::getRenderStats(void)
	{
		return s_rendererData.renderStats;
	}
	
	void Renderer2D::resetStats(void)
	{
		memset(&s_rendererData.renderStats, 0, sizeof(Renderer2D::tRenderStats));
	}

	void Renderer2D::drawQuad(const Vec2& pos, int32_t drawLayer, const Vec2& size, const Color& color)
	{
		if (s_rendererData.indexCount >= MaxIndexCount)
		{
			Renderer2D::endBatch();
			Renderer2D::flush();
			Renderer2D::beginBatch();
		}

		float texIndex = 0.0f;

		s_rendererData.bufferPtr->color = color.getNormalizedColor();
		s_rendererData.bufferPtr->texCoords = { 0.0f, 1.0f };
		s_rendererData.bufferPtr->texIndex = texIndex;
		s_rendererData.bufferPtr->position = { pos, (float)drawLayer };
		s_rendererData.bufferPtr++;

		s_rendererData.bufferPtr->color = color.getNormalizedColor();
		s_rendererData.bufferPtr->texCoords = { 1.0f, 1.0f };
		s_rendererData.bufferPtr->texIndex = texIndex;
		s_rendererData.bufferPtr->position = { pos.x + size.x, pos.y, (float)drawLayer };
		s_rendererData.bufferPtr++;

		s_rendererData.bufferPtr->color = color.getNormalizedColor();
		s_rendererData.bufferPtr->texCoords = { 1.0f, 0.0f };
		s_rendererData.bufferPtr->texIndex = texIndex;
		s_rendererData.bufferPtr->position = { pos.x + size.x, pos.y + size.y, (float)drawLayer };
		s_rendererData.bufferPtr++;

		s_rendererData.bufferPtr->color = color.getNormalizedColor();
		s_rendererData.bufferPtr->texCoords = { 0.0f, 0.0f };
		s_rendererData.bufferPtr->texIndex = texIndex;
		s_rendererData.bufferPtr->position = { pos.x, pos.y + size.y, (float)drawLayer };
		s_rendererData.bufferPtr++;

		s_rendererData.indexCount += 6;
		s_rendererData.renderStats.quadCount++;
	}

    void Renderer2D::drawQuad(const Vec2& pos, int32_t drawLayer, const Vec2& size, ResourceID texture, TextureAtlasIndex tile_index)
	{
		if (s_rendererData.indexCount >= MaxIndexCount || s_rendererData.texSlotIndex >= MaxTextures)
		{
			Renderer2D::endBatch();
			Renderer2D::flush();
			Renderer2D::beginBatch();
		}

		if (s_rendererData.currentTexResID != texture || s_rendererData.currentTex == nullptr || s_rendererData.currentTexResID == ResourceManager::InvalidResource)
		{
			s_rendererData.currentTex = &(ResourceManager::getTexture(texture));
			if (s_rendererData.currentTex->isInvalid()) return; //TODO: Error
			s_rendererData.currentTexResID = texture;
			for (uint32_t i = 1; i < s_rendererData.texSlotIndex; i++)
			{
				if (s_rendererData.texSlots[i] == s_rendererData.currentTex->getOpenGLID())
				{
					s_rendererData.currentTexIndex = (float)i;
					break;
				}
			}

			if (s_rendererData.currentTexIndex == 0.0f)
			{
				s_rendererData.currentTexIndex = (float)s_rendererData.texSlotIndex;
				s_rendererData.texSlots[s_rendererData.texSlotIndex] = s_rendererData.currentTex->getOpenGLID();
				s_rendererData.texSlotIndex++;
			}
		}
		
		Color color { 255 };
		auto texCoords = s_rendererData.currentTex->getTile(tile_index);

		s_rendererData.bufferPtr->color = color.getNormalizedColor();
		s_rendererData.bufferPtr->texCoords = texCoords.topLeft;
		s_rendererData.bufferPtr->texIndex = s_rendererData.currentTexIndex;
		s_rendererData.bufferPtr->position = { pos,  (float)drawLayer };
		s_rendererData.bufferPtr++;

		s_rendererData.bufferPtr->color = color.getNormalizedColor();
		s_rendererData.bufferPtr->texCoords = texCoords.topRight;
		s_rendererData.bufferPtr->texIndex = s_rendererData.currentTexIndex;
		s_rendererData.bufferPtr->position = { pos.x + size.x, pos.y, (float)drawLayer };
		s_rendererData.bufferPtr++;

		s_rendererData.bufferPtr->color = color.getNormalizedColor();
		s_rendererData.bufferPtr->texCoords = texCoords.bottomRight;
		s_rendererData.bufferPtr->texIndex = s_rendererData.currentTexIndex;
		s_rendererData.bufferPtr->position = { pos.x + size.x, pos.y + size.y, (float)drawLayer };
		s_rendererData.bufferPtr++;

		s_rendererData.bufferPtr->color = color.getNormalizedColor();
		s_rendererData.bufferPtr->texCoords = texCoords.bottomLeft;
		s_rendererData.bufferPtr->texIndex = s_rendererData.currentTexIndex;
		s_rendererData.bufferPtr->position = { pos.x, pos.y + size.y, (float)drawLayer };
		s_rendererData.bufferPtr++;

		s_rendererData.indexCount += 6;
		s_rendererData.renderStats.quadCount++;
	}

	void Renderer2D::drawLine(const Vec2& start, const Vec2& end, int32_t drawLayer, float thickness, const Color& color)
	{
		if (s_rendererData.indexCount >= MaxIndexCount)
		{
			Renderer2D::endBatch();
			Renderer2D::flush();
			Renderer2D::beginBatch();
		}

		float texIndex = 0.0f;

		Vec2 unitDirection = (start - end).normalize();
		Vec2 unitPerpendicular(-unitDirection.y, unitDirection.x);
		Vec2 offset = unitPerpendicular * (thickness / 2.f);

		s_rendererData.bufferPtr->color = color.getNormalizedColor();
		s_rendererData.bufferPtr->texCoords = { 0.0f, 1.0f };
		s_rendererData.bufferPtr->texIndex = texIndex;
		s_rendererData.bufferPtr->position = { start + offset,  (float)drawLayer };
		s_rendererData.bufferPtr++;

		s_rendererData.bufferPtr->color = color.getNormalizedColor();
		s_rendererData.bufferPtr->texCoords = { 1.0f, 1.0f };
		s_rendererData.bufferPtr->texIndex = texIndex;
		s_rendererData.bufferPtr->position = { end + offset, (float)drawLayer };
		s_rendererData.bufferPtr++;

		s_rendererData.bufferPtr->color = color.getNormalizedColor();
		s_rendererData.bufferPtr->texCoords = { 1.0f, 0.0f };
		s_rendererData.bufferPtr->texIndex = texIndex;
		s_rendererData.bufferPtr->position = { end - offset, (float)drawLayer };
		s_rendererData.bufferPtr++;

		s_rendererData.bufferPtr->color = color.getNormalizedColor();
		s_rendererData.bufferPtr->texCoords = { 0.0f, 0.0f };
		s_rendererData.bufferPtr->texIndex = texIndex;
		s_rendererData.bufferPtr->position = { start - offset,  (float)drawLayer };
		s_rendererData.bufferPtr++;

		s_rendererData.indexCount += 6;
		s_rendererData.renderStats.quadCount++;
	}
	
	Shader& Renderer2D::bindShader(ResourceID shader)
	{
		if (shader != s_rendererData.currentShaderResID || s_rendererData.currentShader == nullptr || s_rendererData.currentShaderResID == ResourceManager::InvalidResource)
		{
			s_rendererData.currentShader = &(ResourceManager::getShader(shader));
			if (s_rendererData.currentShader->isInvalid()) return (Shader&)BaseObject::InvalidRef(); //TODO: Error
			s_rendererData.currentShaderResID = shader;
			if (s_rendererData.indexCount > 0)
			{
				Renderer2D::endBatch();
				Renderer2D::flush();
			}
			s_rendererData.currentShader->bind();
			Renderer2D::beginBatch();
			return *s_rendererData.currentShader;
		}
		return *s_rendererData.currentShader;
	}

	void Renderer2D::drawSingle(const VertexArray& vao, const Shader& shader)
	{
		shader.bind();
		vao.bind();
		GLCall(glDrawElements(GL_TRIANGLES, vao.getElementCount(), GL_UNSIGNED_INT, 0));
		// GLCall(glDrawElements(GL_TRIANGLES, vao.getElementCount(), GL_UNSIGNED_INT, 0));
	}
}