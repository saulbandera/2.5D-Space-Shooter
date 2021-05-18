#ifndef _GAME_OBJECT_H
#define _GAME_OBJECT_H

#include <graphics/mesh_instance.h>
#include <box2d/Box2D.h>
#include <stdio.h>     
#include <stdlib.h>
#include <time.h> 
enum OBJECT_TYPE
{
	PLAYER,
	ENEMY,
	BULLET
};

class GameObject : public gef::MeshInstance
{


public:

	void UpdateFromSimulation(const b2Body* body);
	void MyCollisionResponse();

	inline void set_type(OBJECT_TYPE type) { type_ = type; }
	inline OBJECT_TYPE type() { return type_; }
	float y_pos = 0.f;
private:
	OBJECT_TYPE type_;
};

class Cube : public GameObject
{
public:
	Cube();
	void DecrementHealth();
};

#endif // _GAME_OBJECT_H