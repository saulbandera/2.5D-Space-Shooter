#pragma once
#include "ModelLoader.h"
#include "game_object.h"
#include "Bullet.h"
class Player 
{
public:
	
	Player() = default;
	Player(b2World* world, PrimitiveBuilder* primitive_builder);

	// Getters and setters
	void setPosition(b2Vec2);
	b2Vec2 getPosition() { return player_body_->GetTransform().p; }
	std::vector<Bullet> getBullets();
	void setKilled(bool killed) { is_killed_ = killed; }
	bool getKilled() { return is_killed_; }
	// Functions
	void init(gef::Platform& platform);
	void shoot(const float& dt, gef::Keyboard* input, gef::InputManager* in_manager, gef::Platform& plat);
	void update(float dt);
	void handle_input(float dt, gef::Keyboard* input, gef::InputManager* in_manager, gef::Platform& plat);
	void render_player( gef::Renderer3D *renderer_3d_);

	std::vector<Bullet> bullets_;

private:

	float shoot_timer, shoot_timer_max;
	float dt_multiplier;
	float max_velocity_;
	float player_acceleration_, player_deceleration_;
	float fire_rate_;
	bool is_killed_ = false;
	GameObject player_;
	gef::Scene* player_model_;
	gef::MeshInstance player_mesh_;
	gef::Renderer3D* player_renderer_;
	b2Body* player_body_;
	PrimitiveBuilder* primitive_builder_;
	b2World* world_;
	b2Vec2 velocity_;
	b2Vec2 direction_;
	Bullet bullet_;

};