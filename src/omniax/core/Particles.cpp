#include "Particles.hpp"
#include <omniax/utils/Defines.hpp>
#include <omniax/utils/Utils.hpp>
#include <omniax/core/Renderer2D.hpp>
#include <omniax/utils/Random.hpp>

namespace ox
{
	void Particle::setup(tParticleInfo partInfo)
	{
		maxVelocity = 5.0f;
		velocity = { 0, 0 };
		acceleration = { 0, 0 };
		velocityDamping = { 0, 0 };
		
		float angle = partInfo.angle;
		if (partInfo.allDirectionos)
			angle = Random::getf32(0.0f, 360.0f);
		float dirVar = angle * partInfo.randomDirection;
		angle += Random::getf32(-dirVar, dirVar);

		float speedVar = partInfo.speed * partInfo.randomSpeed;
		float speed = partInfo.speed + Random::getf32(-speedVar, speedVar);

		float rad = DEG_TO_RAD(angle);
		velocity = { speed * std::cos(rad), -speed * std::sin(rad) };
		Vec2 velVar { velocity.x * partInfo.randomVelocity.x, velocity.y * partInfo.randomVelocity.y };
		velocity.x += Random::getf32(-velVar.x, velVar.x);
		velocity.y += Random::getf32(-velVar.y, velVar.y);

		float lifeVar = partInfo.lifeSpan * partInfo.randomLifeSpan;
		life = partInfo.lifeSpan;
		life += Random::getf32(-lifeVar, lifeVar);

		color = partInfo.color;
		float alphaVar = color.a * partInfo.randomAlpha;
		color.a += Random::geti8(-(int8_t)alphaVar, (int8_t)alphaVar);
		alpha = 0.0f;
		m_curr_alpha = 0.0f;

		size = partInfo.size;
		Vec2 sizeVar { size.x * partInfo.randomSize.x, size.y * partInfo.randomSize.y };
		size.x += Random::getf32(-sizeVar.x, sizeVar.x);
		if (partInfo.square)
			size.y = size.x;
		else
			size.y += Random::getf32(-sizeVar.y, sizeVar.y);

		if (partInfo.randomDamping)
		{
			velocityDamping.x += Random::getf32(0, partInfo.damping.x);
			velocityDamping.y += Random::getf32(0, partInfo.damping.y);
		}

		rotationStep = partInfo.rotationStep;
		float rotVar = rotationStep * partInfo.randomRotation;
		rotationStep += Random::getf32(-rotVar, rotVar);

		texture = partInfo.texture;
		tileIndex = partInfo.tileIndex;

		transform = Transform2D();
		transform.centeredOrigin = true;
		m_ready = true;
		m_dead = false;

		m_fade_in = true;
		fadeIn = partInfo.fadeIn;
		if (fadeIn)
		{
			alpha = color.a;
			m_alpha_dec = color.a / life;
			color.a = 0;
			life /= 2.0f;
		}
		else
			m_alpha_dec = color.a / life;

		fullLife = life;
		fulLAlpha = color.a;
		m_fade_in_mult = (partInfo.lifeSpan / 100.0f);
		colorRamp = partInfo.colorRamp;

		emit_light = partInfo.emit_light;
		if (emit_light)
		{
			light_source.position = position;
			light_source.color = color;
			light_source.is_dead = false;
			light_source.is_on = true;
			light_source.size = partInfo.light_size;
			light_source.texture = partInfo.light_texture;
			light_size_mlt_threshold = partInfo.light_size_mlt_threshold;
		}
		else
		{
			light_source.is_dead = true;
			light_source.is_on = false;
		}
	}

	void Particle::beforeUpdate(void)
	{
		if (isDead()) return;
		transform.rotation += Random::getf32(0.0f, rotationStep);
		if (transform.rotation > 360.0f) transform.rotation -= 360.0f;
		if (colorRamp.count() > 0)
		{
			color.r = colorRamp.current().r;
			color.g = colorRamp.current().g;
			color.b = colorRamp.current().b;
			colorRamp.update();
		}
		if (m_fade_in && fadeIn)
		{
			m_curr_alpha += (m_alpha_dec * m_fade_in_mult);
			color.a = m_curr_alpha;
			if (m_curr_alpha >= alpha)
			{
				m_fade_in = false;
				color.a = alpha;
			}
			return;
		}
		alpha -= m_alpha_dec * 2.0f;
		life--;
		if (alpha <= 0 || life <= 0)
		{
			color.a = 0;
			kill();
		}
		else color.a = std::round(alpha);
	}

	void Particle::afterUpdate(void)
	{
		transform.translation = position;

		if (emit_light)
		{
			auto& lightRef = LightingManager::getLightSource(lightID);
			lightRef.position = position;
			lightRef.color = color;
			if (!m_fade_in)
			{
				float sz_mlt = (life / fullLife) + light_size_mlt_threshold;
				lightRef.size *= sz_mlt;
			}
		}
	}

	void Particle::kill(void)
	{
		 m_dead = true;
		 if (emit_light)
		 {
			auto& lightRef = LightingManager::getLightSource(lightID);
			lightRef.is_dead = true;
			lightRef.is_on = false;
		 }
	}



	ParticleEmitter::ParticleEmitter(void)
	{
		invalidate();
	}

	ParticleEmitter::ParticleEmitter(Rectangle emissionRect, uint32_t maxParticles)
	{
		create(emissionRect, maxParticles);
	}
	
	ParticleEmitter::ParticleEmitter(Vec2 position, uint32_t maxParticles)
	{
		create({ position, 1, 1 }, maxParticles);
	}
	
	ParticleEmitter& ParticleEmitter::create(Rectangle emissionRect, uint32_t maxParticles)
	{
		setEmissionRect(emissionRect);
		m_particles.resize(maxParticles);
		for (auto& part : m_particles)
			part.kill();
		m_particleCount = maxParticles;
		m_currentPathValue = 0.0f;

		useParticleTransform(false);
		enablePath(false);

		setTypeName("ox::ParticleEmitter");
		validate();
		setVisible(false);
		return *this;
	}

	void ParticleEmitter::draw(void)
	{
		if (isInvalid()) return;
		if (m_path.isEnabled() && m_path.exists() && m_path.isEditable())
		{
			m_path.draw();
			ox::Renderer2D::drawQuad(ox::Renderer2D::getStaticQuad(m_currentPathPoint.position, { 10.0f, 10.0f }), { 10, 50, 200, 150 });
		}
		for (uint32_t i = 0; i < m_particleCount; i++)
		{
			auto& part = m_particles[i];
			if (part.isDead()) continue;
			if (m_useParticleTransform)
				ox::Renderer2D::drawQuad({ 0.0f, 0.0f }, part.size, part.texture, part.transform, part.tileIndex, part.color);
			else
				ox::Renderer2D::drawQuad(ox::Renderer2D::getStaticQuad(part.position, part.size, true), part.color, part.texture, part.tileIndex);
		}
	}
	
	void ParticleEmitter::update(const Vec2& force)
	{
		if (isInvalid()) return;
		if (m_path.isEnabled() && m_path.exists())
		{
			m_currentPathValue += m_pathStep;
			if (m_currentPathValue >= m_path.getTotalLength())
				m_currentPathValue -= m_path.getTotalLength();
			float fOffset = m_path.getNormalisedOffset(m_currentPathValue);
			m_currentPathPoint = m_path.getPoint(fOffset);
			setEmissionRect({ m_currentPathPoint.position, 10.0f, 10.0f });
		}
		for (uint32_t i = 0; i < m_particleCount; i++)
		{
			auto& part = m_particles[i];
			if (part.isDead()) continue;
			if (m_useTileArray && m_tileArray.size() > 0)
			{
				float tile = (float)m_tileArray.size() / part.fullLife;
				tile *= part.life;
				uint32_t i = m_tileArray.size() - (uint32_t)std::round(tile);
				if (i < 0) i = 0;
				else if (i >= m_tileArray.size()) i = m_tileArray.size() - 1;
				part.tileIndex = m_tileArray[i];
			}
			if (m_workingRect.w != 0 && m_workingRect.h != 0)
			{
				if (part.position.x + part.size.x < m_workingRect.x ||
					part.position.y + part.size.y < m_workingRect.y ||
					part.position.x > m_workingRect.x + m_workingRect.w ||
					part.position.y > m_workingRect.y + m_workingRect.h)
				{
					part.kill();
					continue;
				}
			}
			part.applyForce(force);
			part.physicsUpdate();
		}
	}

	void ParticleEmitter::emit(tParticleInfo partInfo, uint32_t count)
	{
		if (isInvalid()) return;
		uint32_t index = 0;
		partInfo.angle = getTransform().rotation;
		for (auto& part : m_particles)
		{
			if (part.isDead())
			{
				part.position = getEmissionRect().getPosition() + getRandomEmissionPoint();
				part.setup(partInfo);
				if (partInfo.emit_light)
				{
					if (Random::getf32() >= (1.0f - partInfo.light_emit_chance))
						part.lightID = LightingManager::addLightSource(part.light_source);
				}
				count--;
				if (count == 0) return;
			}
			index++;
		}
	}
	
	void ParticleEmitter::emit(uint32_t count)
	{
		if (isInvalid()) return;
		emit(m_defaultParticle, count);
	}

	void ParticleEmitter::setDefaultParticleInfo(tParticleInfo info)
	{
		m_defaultParticle = info;
	}

	void ParticleEmitter::addTilesToArray(const std::vector<TextureAtlasIndex>& array)
	{
		for (const auto& tile : array)
			m_tileArray.push_back(tile);
	}

	Vec2 ParticleEmitter::getRandomEmissionPoint(void)
	{
		return Random::getVec2({ 0, getEmissionRect().w }, { 0, getEmissionRect().h });
	}



	tParticleInfo ParticleFactory::basicFireParticle(TextureID texture)
	{
		ox::tParticleInfo info;
		info.texture = texture.texture;
		info.tileIndex = texture.tile;
		info.speed = 0.45f;
		info.randomVelocity = { 0.2f, 0.3f };
		info.randomDirection = 0.24f;
		info.randomAlpha = 0.45f;
		info.randomSize = { 0.8f, 0.8f };
		info.size = { 40.0f, 40.0f };
		info.lifeSpan = 600.0f;
		info.addColorToGradient({ 255, 255, 255 }, { 255, 193, 31 }, 0.05f);
		info.addColorToGradient({ 255, 247, 93 }, { 255, 193, 31 }, 0.05f);
		info.addColorToGradient({ 255, 193, 31 }, { 254, 101, 13 }, 0.1f);
		info.addColorToGradient({ 254, 101, 13 }, { 243, 60, 4 }, 0.4f);
		info.addColorToGradient({ 243, 60, 4 }, { 218, 31, 5 }, 0.3f);
		info.addColorToGradient({ 218, 31, 5 }, { 161, 1, 0 }, 0.05f);
		info.addColorToGradient({ 161, 1, 0 }, { 161, 1, 0 }, 0.05f);
		info.emit_light = true;
		info.light_texture = { ResourceManager::getDefaultLightTexture(), 0 };
		info.light_emit_chance = 0.3f;
		info.light_size = { 100, 100 };
		info.light_size_mlt_threshold = 0.05f;
		return info;
	}
	
	tParticleInfo ParticleFactory::basicSnowParticle(TextureID texture)
	{
		ox::tParticleInfo info;
		info.texture = texture.texture;
		info.tileIndex = texture.tile;
		info.speed = 1.2f;
		info.color = { 132, 165, 216 };
		info.randomVelocity.x = 0.0f;
		info.randomDirection = 0.0f;
		info.size = { 16.0f, 16.0f };
		info.randomSize = { 0.3f, 0.3f };
		info.lifeSpan = 10000;
		info.randomDamping = true;
		info.randomLifeSpan = 0.0f;
		info.damping = { 0.08f, 0.0f };
		info.randomAlpha = 0.1f;
		return info;
	}

	ParticleEmitter ParticleFactory::basicFireEmitter(TextureID texture, Vec2 position, uint32_t pre_emit_cycles)
	{
		ParticleEmitter emitter(Rectangle(position, 10.0f, 10.0f), 1000);
		emitter.rotate(90.0f);
		emitter.setDefaultParticleInfo(ParticleFactory::basicFireParticle(texture));
		ParticleFactory::__pre_emit(emitter, pre_emit_cycles, { 0.0f, 0.002f });
		return emitter;
	}

	ParticleEmitter ParticleFactory::basicSnowEmitter(TextureID texture, Vec2 windowSize, uint32_t pre_emit_cycles)
	{
		ParticleEmitter emitter(Rectangle(0, 0, windowSize.x, 1.0f), 2000);
		emitter.rotate(-90.0f);
		emitter.setDefaultParticleInfo(ParticleFactory::basicSnowParticle(texture));
		emitter.setWorkingRectangle({ 0.0f, 0.0f, windowSize });
		ParticleFactory::__pre_emit(emitter, pre_emit_cycles, { 0.002, 0.09 });
		return emitter;
	}

	void ParticleFactory::__pre_emit(ParticleEmitter& emitter, uint32_t pre_emit_cycles, Vec2 rand_force_range)
	{
		uint32_t current = 0;
		Vec2 wind { 0.0f, 0.0f };
		for (uint32_t i = 0; i < pre_emit_cycles; i++)
		{
			emitter.emit(Random::geti32(1, 2));
			if (current++ > 30)
			{
				current = 0;
				wind.x = Random::getf32(rand_force_range.x, rand_force_range.y);
			}
			emitter.update(wind);
		}
	}
}