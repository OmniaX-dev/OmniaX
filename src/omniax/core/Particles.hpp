#ifndef __PARTICLES_HPP__
#define __PARTICLES_HPP__

#include <omniax/physics/PhysicsObject.hpp>
#include <omniax/core/GameObject.hpp>
#include <omniax/core/DataStructures.hpp>
#include <omniax/graphics/Color.hpp>
#include <omniax/core/ResourceManager.hpp>
#include <omniax/utils/Spline.hpp>

namespace ox
{
	struct tColorInterpolator
	{
		Color start;
		Color end;
		float length { 60.0f };
		uint32_t currentFrame { 0 };
		Color current;
		bool done { false };
		float percent { 0.2f };

		inline tColorInterpolator(void) {  }

		inline tColorInterpolator(Color st, Color en)
		{
			start = st;
			end = en;
			length = 0.0f;
			current = start;
		}

		inline void reset(void)
		{
			currentFrame = 0;
			current = start;
			done = false;
		}

		inline void updateLength(float new_total_length)
		{
			length = percent * new_total_length;
		}

		inline void update(void)
		{
			if (done || length == 0.0f) return;
			currentFrame++;
			if (currentFrame >= length)
			{
				done = true;
				return;
			}
			float t = (float)currentFrame / (length - 1);
			uint8_t r = (int)std::round(std::lerp(start.r, end.r, t));
			uint8_t g = (int)std::round(std::lerp(start.g, end.g, t));
			uint8_t b = (int)std::round(std::lerp(start.b, end.b, t));
			uint8_t a = (int)std::round(std::lerp(start.a, end.a, t));
			current.set(r, g, b, a);
		}
	};

	class ColorRamp
	{
		public:
			inline void update(void)
			{
				if (done) return;
				// if (m_colors.size() == 0) return;
				m_colors[m_currentColor].update();
				if (m_colors[m_currentColor].done)
					m_currentColor++;
				if (m_currentColor >= m_colors.size())
					done = true;
			}

			inline void reset(void)
			{
				m_currentColor = 0;
				for (auto& inter : m_colors)
					inter.reset();
				done = false;
			}

			inline Color current(void)
			{
				if (m_colors.size() == 0)
					return { 0, 0, 0, 0 };
				if (done) return m_colors[m_colors.size() - 1].current;
				return m_colors[m_currentColor].current;
			}

			inline uint32_t count(void) { return m_colors.size(); }

		public:
			std::vector<tColorInterpolator> m_colors;
			uint32_t m_currentColor { 0 };
			bool done { false };
	};

	struct tParticleInfo
	{
		float lifeSpan { 100 };

		Vec2 randomVelocity { 0.2f, 0.2f };
		float randomLifeSpan { 0.2f };
		float randomDirection { 0.2f };
		float randomSpeed { 0.2f };
		float randomAlpha { 0.2f };
		float randomRotation { 0.2f };
		Vec2 randomSize { 0.2f, 0.2f  };
		bool randomDamping { true };
		
		Vec2 damping { 0.005f, 0.005f };
		ResourceID texture { ResourceManager::InvalidResource };
		TextureAtlasIndex tileIndex { ox::Texture::FullTextureCoords };
		ox::Color color { 120, 120, 120, 80 };
		Vec2 size { 16.0f, 16.0f };
		float rotationStep { 1.0f };

		bool fadeIn { true };
		bool allDirectionos { false };
		bool square { true };
	
		float angle { 0.0f };
		float speed { 1.0f };

		ColorRamp colorRamp;

		inline void addColorToGradient(Color start, Color end, float percent)
		{
			colorRamp.m_colors.push_back({ start, end });
			colorRamp.m_colors[colorRamp.m_colors.size() - 1].percent = percent;
			colorRamp.m_colors[colorRamp.m_colors.size() - 1].updateLength(lifeSpan);
		}
	};

	class Particle : public PhysicsObject
	{
		public:
			inline Particle(void) { m_ready = false; }
			void setup(tParticleInfo partInfo);
			void beforeUpdate(void) override;
			void afterUpdate(void) override;
			inline bool isReady(void) { return m_ready; }
			inline bool isDead(void) { return m_dead; }
			inline void kill(void) { m_dead = true; }

		private:
			float m_alpha_dec { 0 };
			bool m_ready { false };
			bool m_dead { false };
			bool m_fade_in { true };
			float m_curr_alpha { 0.0f };
			float m_fade_in_mult { 1.0f };

		public:
			float life { 100 };
			float fullLife { 100 };
			uint8_t fulLAlpha;
			float rotationStep { 1.0f };
			ox::Color color { 10, 110, 255 };
			Vec2 size { 30.0f, 30.0f };
			ResourceID texture { ResourceManager::InvalidResource };
			TextureAtlasIndex tileIndex { ox::Texture::FullTextureCoords };
			Transform2D transform;
			float alpha { 0.0f }; 

			bool dead { false };
			bool fadeIn { true };

			ColorRamp colorRamp;
	};

	class ParticleEmitter : public GameObject
	{
		public:
			ParticleEmitter(void);
			ParticleEmitter(Rectangle emissionRect, uint32_t maxParticles = 400);
			ParticleEmitter(Vec2 position, uint32_t maxParticles = 400);
			ParticleEmitter& create(Rectangle emissionRect, uint32_t maxParticles = 400);

			void draw(const RenderTarget& target) override;
			void update(const Vec2& force = { 0.0f, 0.0f }) override;

			void emit(tParticleInfo partInfo, uint32_t count = 1);
			void emit(uint32_t count = 1);

			void setDefaultParticleInfo(tParticleInfo info);
			inline tParticleInfo& getDefaultParticleInfo(void) { return m_defaultParticle; }
			inline void setEmissionRect(Rectangle rect) { setBaseRect(rect); }
			inline Rectangle getEmissionRect(void) { return getBaseRect(); }
			inline void setWorkingRectangle(Rectangle rect) { m_workingRect = rect; }
			inline Rectangle getWorkingRectangle(void) { return m_workingRect; }
			inline bool isParticleTransformEnabled(void) { return m_useParticleTransform; }
			inline void useParticleTransform(bool p = true) { m_useParticleTransform = p; }
			inline void setMaxParticleCount(uint32_t maxParticles) { m_particleCount = maxParticles; m_particles.resize(m_particleCount); }
			inline uint32_t getMaxParticleCount(void) { return m_particleCount; }
			inline void useTileArray(bool u = true) { m_useTileArray = u; }
			inline bool isTileArrayUsed(void) { return m_useTileArray; }
			void addTilesToArray(const std::vector<TextureAtlasIndex>& array);

			// inline void setPath(Spline& path) { m_path = path; m_path.enable(); }
			inline void enablePath(bool e = true) { m_path.enable(e); }
			inline void addPathPoint(Vec2 point) { m_path.addPoint(point); }
			inline void enableEditablePath(bool e = true) { m_path.setEditable(e); if (e) m_path.connectSignals(); }

		private:
			Vec2 getRandomEmissionPoint(void);

		private:
			tParticleInfo m_defaultParticle;
			std::vector<Particle> m_particles;
			uint32_t m_particleCount;
			Rectangle m_workingRect;
			std::vector<TextureAtlasIndex> m_tileArray;

			Spline m_path;
			float m_currentPathValue { 0.0f };
			float m_pathStep { 15.0f };
			ox::tSplineNode m_currentPathPoint { { 0.0f, 0.0f }, 0.0f };
			bool m_useParticleTransform { true };
			bool m_useTileArray { false };
	};

	class ParticleFactory
	{
		public:
			static tParticleInfo basicFireParticle(ResourceID texture = ResourceManager::InvalidResource);
			static tParticleInfo basicSnowParticle(ResourceID texture = ResourceManager::InvalidResource);
	
			static ParticleEmitter basicFireEmitter(ResourceID texture, Vec2 position);
			static ParticleEmitter basicSnowEmitter(ResourceID texture, Vec2 windowSize);
	};
}

#endif