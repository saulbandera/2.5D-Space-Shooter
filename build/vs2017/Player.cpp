#include "Player.h"



Player::Player(b2World* world, PrimitiveBuilder* primitive_builder){
	dt_multiplier = 62.f;
	fire_rate_ = 10.f;
	player_acceleration_ = 9.f;
	player_deceleration_ = 4.f;
	max_velocity_ = 140.f;
	world_ = world;
	primitive_builder_ = primitive_builder;
	velocity_.Set(0.f,0.f);
	shoot_timer_max = 10.f;
	shoot_timer = shoot_timer_max;

}
 
#pragma region Getters&Setters

void Player::setPosition(b2Vec2 position)
{
	player_body_->SetTransform(position, 0);
	player_.UpdateFromSimulation(player_body_);
}


std::vector<Bullet> Player::getBullets()
{
	return bullets_;
}
#pragma endregion


/////////   INIT   /////////
void Player::init(gef::Platform& platform)
{
	const char* player_asset_filename = "spaceship1.scn";
	player_model_ = LoadSceneAssets(platform, player_asset_filename);
	if (player_model_)
	{
		player_.set_mesh(GetMeshFromSceneAssets(player_model_));
	}
	else
	{
		gef::DebugOut("Scene file %s failed to load\n", player_asset_filename);
	}

	b2BodyUserData user_data_;
	user_data_.pointer = 2;

	b2BodyDef player_body_def;
	player_body_def.type = b2_dynamicBody;
	player_body_def.userData = user_data_;
	player_body_ = world_->CreateBody(&player_body_def);

	//Create the shape for the player
	b2CircleShape player_shape;
	player_shape.m_radius = 0.01f;

	// Create Fixture 
	b2FixtureDef player_fixture_def;
	player_fixture_def.shape = &player_shape;
	player_fixture_def.density = 1.0f;

	// Create the fixture on the rigid body
	player_body_->CreateFixture(&player_fixture_def);

	//update visuals from simulation data 
	player_.UpdateFromSimulation(player_body_);
}


/////////   SHOOT   /////////
void Player::shoot(const float& dt, gef::Keyboard* input, gef::InputManager* in_manager, gef::Platform& plat)
{
	const gef::TouchInputManager* touch_input = in_manager->touch_manager();
	gef::Keyboard* keyboardManager = input;

	if (touch_input->is_button_down(0) && this->shoot_timer >= this->shoot_timer_max) {
		bullet_ = Bullet(world_, primitive_builder_);
		bullet_.setPosition(player_body_->GetPosition(), b2Rot(0));
		bullet_.shoot(60, b2Vec2(0, -1));
		bullets_.push_back(bullet_); 
		this->shoot_timer = 0; // Reset timer
	}
}




/////////   UPDATE   /////////
void Player::update(float dt)
{

	if (is_killed_) {
		gef::DebugOut("PLAYER KILLED\n");
	}
	for (size_t i = 0; i < bullets_.size(); i++) {
		bullets_[i].update();
	}

	this->shoot_timer_max = 10.f;
	if (this->shoot_timer < this->shoot_timer_max) {
		this->shoot_timer += 1.f * dt * this->dt_multiplier;
	}
}

/////////   HANDLE INPUT   /////////
void Player::handle_input(float dt, gef::Keyboard* input, gef::InputManager* in_manager, gef::Platform& plat)
{
	player_.UpdateFromSimulation(player_body_);
	gef::Keyboard* keyboardManager = input;

	if (keyboardManager->IsKeyDown(gef::Keyboard::KC_A)) { // LEFT
		direction_.Set(-1, 0);
		if (velocity_.x > -this->max_velocity_ && direction_.x < 0)
			velocity_.x += direction_.x * player_acceleration_ * dt * dt_multiplier;
	}

	if (keyboardManager->IsKeyDown(gef::Keyboard::KC_D)) { // RIGHT
		direction_.Set(1, 0);
		if (velocity_.x < this->max_velocity_ && direction_.x > 0)
			velocity_.x += direction_.x * player_acceleration_ * dt * dt_multiplier;
	}

	////// Deceleration 
	if (this->velocity_.x > 0) { // moving right
		this->velocity_.x -= player_deceleration_ * dt * dt_multiplier;
		if (this->velocity_.x < 0)
			this->velocity_.x = 0;
		else {
			player_body_->SetTransform(this->getPosition(), -velocity_.x / 490);
		}
		
	}

	else if (this->velocity_.x < 0) {// moving left
		this->velocity_.x += player_deceleration_ * dt * dt_multiplier;
		if (this->velocity_.x > 0)
			this->velocity_.x = 0;
		else {
			player_body_->SetTransform(this->getPosition(), -velocity_.x / 490);
		}
	
	}
	player_body_->SetLinearVelocity(velocity_);
	player_.UpdateFromSimulation(player_body_);
	this->shoot(dt, input, in_manager, plat);
}


/////////   RENDER   /////////
void Player::render_player( gef::Renderer3D* renderer_3d_)
{
	player_renderer_ = renderer_3d_;

	for (int i = 0; i < bullets_.size(); i++)
	{
		bullets_[i].render(player_renderer_);
	}
	player_renderer_->DrawMesh(player_);
}
