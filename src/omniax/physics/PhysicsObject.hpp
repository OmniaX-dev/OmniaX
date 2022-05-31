#ifndef __PHYSICS_OBJECT_HPP__
#define __PHYSICS_OBJECT_HPP__

#include <omniax/utils/Geometry.hpp>

namespace ox
{
	class PhysicsObject
	{
		public:
			PhysicsObject(void);
			void applyForce(Vec2 force, float max = 0.0f);
			void physicsUpdate(void);
			inline void setStatic(bool st) { m_static = st; }
			inline bool isStatic(void) { return m_static; }

		protected:
			inline virtual void beforeUpdate(void) {  }
			inline virtual void afterUpdate(void) {  }
			inline virtual void beforeApplyForce(Vec2 force, float max) {  }
			inline virtual void afterApplyForce(Vec2 force, float max) {  }

			inline void skipNextUpdate(bool skip = true) { m_skipUpdate = skip; }
			inline void skipNextApplyForce(bool skip = true) { m_skipApplyForce = skip; }
			inline bool willSkipNextUpdate(void) { return m_skipUpdate; }
			inline bool willSkipNextApplyForce(void) { return m_skipApplyForce; }

		public:
			Vec2 velocity;
			Vec2 position;
			Vec2 acceleration;
			Vec2 velocityDamping;
			float maxVelocity;
			float maxForce;
			float mass;

		private:
			bool m_skipUpdate;
			bool m_skipApplyForce;
			bool m_static;
	};
}

#endif