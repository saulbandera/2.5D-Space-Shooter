#pragma once
#include "ModelLoader.h"
#include "game_object.h"

class Asteroid
{
public:
	Asteroid() = default;
	Asteroid(b2World * world);

	virtual ~Asteroid();

	// Getters and setters
	void setPosition(b2Vec2);
	b2Vec2 getPosition() { return asteroid_body_->GetTransform().p; }
	b2Rot getRotation() { return asteroid_body_->GetTransform().q; }
	b2Body* getBody() { return asteroid_body_; }
	void setHP(int HP) { hp_ = HP; }
	int getHP() { return hp_; }
	// Functions
	void init(gef::Mesh* ast_mesh_);
	void update(float dt, float velocity, b2Vec2 direction);
	void render(gef::Renderer3D * renderer_3d_);

	void setMesh(gef::Mesh* mesh);

private:
	float dt_multiplier;
	float max_velocity_;
	int hp_ = 2;

	GameObject asteroid_;
	gef::MeshInstance asteroid_mesh_;
	gef::Renderer3D* asteroid_renderer_;
	b2Body* asteroid_body_;


	PrimitiveBuilder* primitive_builder_;
	b2World* world_;

};

