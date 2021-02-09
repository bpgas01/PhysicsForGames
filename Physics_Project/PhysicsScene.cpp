#include "PhysicsScene.h"
#include "PhysicsObject.h"
#include "Rigidbody.h"
#include "Sphere.h"
#include "Plane.h"
#include <iostream>
#include <list>

typedef bool (*fn)(PhysicsObject*, PhysicsObject*);

static fn collisionFunctionArray[] =
{
	PhysicsScene::Plane2Plane, PhysicsScene::Plane2Sphere,
	PhysicsScene::Sphere2Plane, PhysicsScene::Sphere2Sphere,
};

PhysicsScene::PhysicsScene() : m_timeStep(0.01f), m_gravity(mathfs::Vector2(0, 0))
{
}

PhysicsScene::~PhysicsScene()
{
	for (auto pActor : m_actors)
	{
		
		delete pActor;
	}
}


void PhysicsScene::ForceDelete()
{

	for (auto p : m_actors)
	{		
		if (p->getShapeID() == SPHERE)
		{
			auto it = std::find(m_actors.begin(), m_actors.end(),p);
			m_actors.erase(it);
			return;
		}

	}

}

void PhysicsScene::addActor(PhysicsObject* a_actor)
{
	m_actors.push_back(a_actor);
}

void PhysicsScene::removeActor(PhysicsObject* a_actor)
{
	auto it = std::find(m_actors.begin(), m_actors.end(),
		a_actor);

	if (it != m_actors.end())
	{
		m_actors.erase(it);
	}
}

void PhysicsScene::update(float deltaTime)
{
	static float accumulatedTime = 0.f;
	accumulatedTime += deltaTime;

	while (accumulatedTime >= m_timeStep)
	{
		for (auto pActor : m_actors)
		{
			pActor->FixedUpdate(m_gravity, m_timeStep);
		}

		accumulatedTime -= m_timeStep;

		CheckForCollision();
	}
}

void PhysicsScene::draw()
{
	for (auto pActor : m_actors)
	{
		pActor->MakeGizmo();
	}
}

void PhysicsScene::DebugScene()
{
	int count = 0;
	for (auto pActor : m_actors)
	{
		std::cout << count << " : ";
		pActor->Debug();
		count++;
	}
}

void PhysicsScene::CheckForCollision()
{
	int actorCount = m_actors.size();
	//

	for (int outer = 0; outer < actorCount; outer++)
	{
		for (int inner = outer + 1; inner < actorCount; inner++)
		{
			PhysicsObject* objOuter = m_actors[outer];
			PhysicsObject* objInner = m_actors[inner];

			int shapeID_out = objOuter->getShapeID();
			int shapeID_inner = objInner->getShapeID();

			int functionIdx = (shapeID_out * SHAPE_COUNT) + shapeID_inner;
			fn collisionFunctPointer = collisionFunctionArray[functionIdx];
			if (collisionFunctPointer != nullptr)
			{
				
				collisionFunctPointer(objOuter, objInner);
				
			}
			
		}
	}
}

bool PhysicsScene::Plane2Plane(PhysicsObject*, PhysicsObject*)
{
	return false;
}

bool PhysicsScene::Plane2Sphere(PhysicsObject* objPlane, PhysicsObject* objSphere)
{
	return Sphere2Plane(objSphere, objPlane);
}

bool PhysicsScene::Sphere2Plane(PhysicsObject* objSphere, PhysicsObject* objPlane)
{
	Sphere* sphere = dynamic_cast<Sphere*>(objSphere);
	Plane* plane = dynamic_cast<Plane*>(objPlane);

    


	// If these have a value, continue below
	if (sphere != nullptr && plane != nullptr)
	{
		mathfs::Vector2 CollisionNormal = plane->GetNormal();


		float sphereToPlane = mathfs::extra::dot(sphere->GetPosition(), CollisionNormal - plane->GetDistance());
		float intersection = sphere->GetRadius() - sphereToPlane;
		 
		float velocityOutOfPlane  = mathfs::extra::dot(sphere->GetVelocity(), CollisionNormal);

		if (intersection > 0 && velocityOutOfPlane < 0)
		{
			mathfs::Vector2 a(-sphere->GetVelocity().x, -sphere->GetVelocity().y);
			
			sphere->ApplyForce(a * sphere->GetMass() * 2.f);
			//objSphere->resetPosition();
			return true;
		}
	}

	return false;
}

bool PhysicsScene::Sphere2Sphere(PhysicsObject* obj1, PhysicsObject* obj2)
{
	Sphere* sphere1 = dynamic_cast<Sphere*>(obj1);
	Sphere* sphere2 = dynamic_cast<Sphere*>(obj2);


	if (sphere1 != nullptr && sphere2 != nullptr)
	{

		float distance = mathfs::extra::distance(sphere1->GetPosition(), sphere2->GetPosition());
		float penetration = sphere1->GetRadius() + sphere2->GetRadius() - distance;
		if (penetration > 0)
		{
			sphere1->ResolveCollision(sphere2);
			return true;
		}
	}

	return false;
}