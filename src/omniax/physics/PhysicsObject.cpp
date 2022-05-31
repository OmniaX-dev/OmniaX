#include "PhysicsObject.hpp"
#include <omniax/utils/Defines.hpp>

namespace ox
{
	PhysicsObject::PhysicsObject(void)
	{
		velocity = { 0, 0 };
		position = { 0, 0 };
		acceleration = { 0, 0 };
		velocityDamping = { 0, 0 };
		maxVelocity = 1.0f;
		maxForce = 2.0f;
		mass = 1.0f;
		m_skipApplyForce = false;
		m_skipUpdate = false;
		m_static = false;
	}

	void PhysicsObject::applyForce(Vec2 force, float max)
	{
		beforeApplyForce(force, max);
		if (willSkipNextApplyForce())
		{
			skipNextApplyForce(false);
			return;
		}
		if (!isStatic())
		{
			if (max == 0.0f) max = maxForce;
			force.divm(mass);
			force.limit(max);
			acceleration.addm(force);
		}
		skipNextApplyForce(false);
		afterApplyForce(force, max);
	}

	void PhysicsObject::physicsUpdate(void)
	{
		beforeUpdate();
		if (willSkipNextUpdate())
		{
			skipNextUpdate(false);
			return;
		}
		velocity.addm(acceleration);
		velocity.limit(maxVelocity);
		position.addm(velocity);
		acceleration.mulm(0);
		velocity.x *= 1.0f - MIN(velocityDamping.x, 0.9999f);
		velocity.y *= 1.0f - MIN(velocityDamping.y, 0.9999f);
		skipNextUpdate(false);
		afterUpdate();
	}
}