#pragma once
#include<iostream>
#include"game_object.h"
#include <system/application.h>
#include <maths/vector2.h>
#include "primitive_builder.h"
#include <graphics/sprite.h>
#include <graphics/mesh_instance.h>
#include <box2d/Box2D.h>
#include<graphics/renderer_3d.h>
#include<input/input_manager.h>
#include<input/keyboard.h>
using std::cout;
class Bullet
{
public:
	Bullet() {};
	Bullet(b2World*, PrimitiveBuilder*);
	void shoot(float, b2Vec2);
	void update();
	void render(gef::Renderer3D*);
	void setPosition(b2Vec2, b2Rot);
	b2Vec2 getPosition() { return projectile_body_->GetTransform().p; }
	b2Rot getRotation() { return projectile_body_->GetTransform().q; }
	b2Body* getBody() { return projectile_body_; }
private:
	//CameraMov FPScamera;
	gef::Renderer3D* renderer_3d_;
	PrimitiveBuilder* primitive_builder_;
	b2World* world_;
	GameObject projectile_;
	b2Body* projectile_body_;
	gef::Material* material;
	gef::Mesh* projectile_mesh_;
	

};
