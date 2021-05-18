#include "Asteroid.h"

Asteroid::Asteroid(b2World* world)
{
	world_ = world;
	
	b2BodyUserData user_data_;
	user_data_.pointer = 1;

	//create a physics body for the projectiles
	b2BodyDef asteroid_body_def;
	asteroid_body_def.type = b2_dynamicBody;
	asteroid_body_def.bullet = true;
	asteroid_body_def.userData = user_data_;
	asteroid_body_def.position = b2Vec2(1000, 8.0f);
	asteroid_body_ = world_->CreateBody(&asteroid_body_def);

	//create the shape for the projectile
	b2CircleShape asteroid_shape;
	asteroid_shape.m_radius = 5.0f;

	// Create Ficture 
	b2FixtureDef asteroid_fixture_def;
	asteroid_fixture_def.shape = &asteroid_shape;
	asteroid_fixture_def.density = 1.0f;

	// Create the fixture on the rigid body
	asteroid_body_->CreateFixture(&asteroid_fixture_def);

	//random asteroid spawn position
	int random_value = rand() % (int)(200) + (int)((-100));
	this->setPosition(b2Vec2((float)random_value, -80));

	asteroid_.UpdateFromSimulation(asteroid_body_);

}

Asteroid::~Asteroid() {}


////////    INIT    ////////  
void Asteroid::init(gef::Mesh* ast_mesh_)
{
	asteroid_.set_mesh(ast_mesh_);
	hp_ = 2;
}


////////    SET POSITION    ////////  
void Asteroid::setPosition(b2Vec2 position)
{

	int random_value = rand() % (int)(30000) + (int)((-33100));
	asteroid_body_->ApplyTorque(random_value, 0);
	asteroid_body_->SetTransform(position, 0);
	asteroid_.UpdateFromSimulation(asteroid_body_);
}


////////    UPDATE    ////////  
void Asteroid::update(float dt, float velocity, b2Vec2 direction)
{
	direction *= velocity*dt;
	asteroid_body_->ApplyLinearImpulseToCenter(direction, true);
	asteroid_.UpdateFromSimulation(asteroid_body_);
}


////////    RENDER    ////////   
void Asteroid::render(gef::Renderer3D* renderer_3d_)
{
	asteroid_renderer_ = renderer_3d_;
	asteroid_renderer_->DrawMesh(asteroid_);
}

void Asteroid::setMesh(gef::Mesh* mesh) {
	asteroid_.set_mesh(mesh);
}
