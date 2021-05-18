#include "Bullet.h"

#include <system/debug_log.h>

Bullet::Bullet(b2World* world, PrimitiveBuilder* primitive_builder)
{
	world_ = world;
	primitive_builder_ = primitive_builder;
	projectile_.set_mesh(primitive_builder_->GetDefaultSphereMesh());

	b2BodyUserData user_data_;
	user_data_.pointer = 5;

	b2BodyDef projectile_body_def;
	projectile_body_def.type = b2_dynamicBody;
	projectile_body_def.bullet = true;
	projectile_body_def.userData = user_data_;
	projectile_body_def.position = b2Vec2(1000, 8.0f);

	projectile_body_ = world_->CreateBody(&projectile_body_def);

	//create the shape for the projectile
	b2CircleShape projectile_shape;
	projectile_shape.m_radius = 0.01f;

	// Create Fixture 
	b2FixtureDef projectile_fixture_def;
	projectile_fixture_def.shape = &projectile_shape;
	projectile_fixture_def.density = 1.0f;

	// Create the fixture on the rigid body
	projectile_body_->CreateFixture(&projectile_fixture_def);

	//update visuals from simulation data 
	projectile_.UpdateFromSimulation(projectile_body_);
	projectile_.y_pos = 1.5f;


}

void Bullet::setPosition(b2Vec2 position, b2Rot rotation)
{
	projectile_body_->SetTransform(position, 0);
	projectile_.UpdateFromSimulation(projectile_body_);
}


////////    SHOOT    ////////  
void Bullet::shoot(float velocity, b2Vec2 direction)
{
	direction *= velocity;
	projectile_body_->ApplyLinearImpulseToCenter(direction, true);
	projectile_.UpdateFromSimulation(projectile_body_);
}


////////    UPDATE    ////////  
void Bullet::update()
{
	projectile_.UpdateFromSimulation(projectile_body_);
}


////////    RENDER    ////////  
void Bullet::render(gef::Renderer3D* render)
{
	b2Vec2 position = projectile_body_->GetTransform().p;
	renderer_3d_ = render;

	renderer_3d_->set_override_material(&primitive_builder_->green_material());
	renderer_3d_->DrawMesh(projectile_);
	renderer_3d_->set_override_material(NULL);
}






