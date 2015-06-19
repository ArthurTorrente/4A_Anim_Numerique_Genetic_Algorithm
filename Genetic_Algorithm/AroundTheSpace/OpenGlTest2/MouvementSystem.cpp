#include "stdafx.h"
#include "MouvementSystem.h"

MouvementSystem::MouvementSystem()
{
}


MouvementSystem::~MouvementSystem()
{
}

void MouvementSystem::update(std::vector<Entity*>* children, int deltaTime)
{
	for (std::vector<Entity*>::iterator iterator = children->begin(); iterator != children->end(); iterator++)
	{
		Entity* entity = *iterator;
		if (entity->get_vertexBuffer() != NULL) {
			

			/*Vector3 v;
			GLfloat len;
			v = normalizeVector3(subtractVector3(entity->get_goal(), entity->get_position()));
			len = length(subtractVector3(entity->get_goal(), entity->get_position()));

			if (len > 0.10f)
			{
				entity->set_velocity(makeVector3((v.x*len) / 50, (v.y*len) / 50, (v.z*len) / 50));
			}
			else {
				entity->set_velocity(makeVector3(0.0f, 0.0f, 0.0f));
				entity->set_position(entity->get_goal());
			}*/

			entity->set_position(addVector3(entity->get_position(), entity->get_velocity()));
			entity->set_scale(addVector3(entity->get_scale(), entity->get_scaleVelocity()));
			entity->set_rotation(addVector3(entity->get_rotation(), entity->get_rotationVelocity()));
		}
		
		
	}
}

MouvementSystem& MouvementSystem::getMouvementSystem()
{
	static MouvementSystem* mouvementSystem = NULL;

	if (mouvementSystem != NULL) {
		mouvementSystem = new MouvementSystem();
	}

	return *mouvementSystem;
}

void MouvementSystem::destroyMouvementSystem()
{
	MouvementSystem* mouvementSystem = &getMouvementSystem();
	delete mouvementSystem;
}


float MouvementSystem::Approach(GLfloat goal, GLfloat Current, float dt){

	GLfloat Difference = goal - Current;

	if (Difference > dt)
	{
		return Current + dt;
	}

	if (Difference < -dt)
	{
		return Current - dt;
	}


	return goal;

}