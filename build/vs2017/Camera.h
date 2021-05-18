#pragma once
#include "Includes.h"

class Camera
{
public:

	gef::InputManager* input;
	Camera(const gef::Keyboard* keyboard);
	void set_position(gef::Vector4 vpos);
	gef::Vector4 get_position();

	void set_lookat(gef::Vector4 vlook);
	gef::Vector4 get_lookat();

	void set_up(gef::Vector4 vup);
	gef::Vector4 get_up();

	void Update();
	void HandleInput(float dt, gef::Keyboard* input, gef::InputManager* in_manager, float width, float height /*, float x, float y*/);
private:


	gef::Vector4 camera_position_;
	gef::Vector4 camera_lookat_;
	gef::Vector4 camera_up_;
	gef::Vector4 camera_right_;
	gef::Vector4 camera_forward_;
	gef::Vector4 camera_rotation_;
	gef::Vector2 mouse_position_;
	float mousePrison_[2]; // to make mouse go back to center screen
	float pitch_ = 0; // rotation around x-axis
	float yaw_ = 0; // rotation around y-axis 
	float roll_ = 0; // rotation around z-axis
	float mouse_y_;
	float mouse_x_;
	float camSpeed_;
	float rotSpeed_;
};