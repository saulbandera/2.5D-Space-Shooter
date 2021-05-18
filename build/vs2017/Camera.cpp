#include "Camera.h"

Camera::Camera(const gef::Keyboard* keyboard)
{
	camera_position_ = { 0, 0, 6 };
	camera_rotation_ = { 0, 0, 0 };
	camSpeed_ = 20.f;
	rotSpeed_ = 80.f;
}


#pragma region Getters&Setters


gef::Vector4 Camera::get_position()
{
	return camera_position_;
}


void Camera::set_position(gef::Vector4 vpos)
{
	camera_position_ = vpos;
}


gef::Vector4 Camera::get_lookat()
{
	return camera_lookat_;
}


void Camera::set_lookat(gef::Vector4 vlook)
{
	camera_lookat_ = vlook;
}


gef::Vector4 Camera::get_up()
{
	return camera_up_;
}


void Camera::set_up(gef::Vector4 vup)
{
	camera_up_ = vup;
}

#pragma endregion GettersSetters

void Camera::Update()
{
	float cosR, cosP, cosY;
	float sinR, sinP, sinY;
	cosY = cosf(yaw_ * 3.1415 / 180);
	cosP = cosf(pitch_ * 3.1415 / 180);
	cosR = cosf(roll_ * 3.1415 / 180);
	sinY = sinf(yaw_ * 3.1415 / 180);
	sinP = sinf(pitch_ * 3.1415 / 180);
	sinR = sinf(roll_ * 3.1415 / 180);
	// forward vectord
	camera_forward_.set_x(sinY * cosP);
	camera_forward_.set_y(sinP);
	camera_forward_.set_z(cosP * -cosY);

	// look at vector
	camera_lookat_ = camera_position_ + camera_forward_;

	// up vector
	camera_up_.set_x(-cosY * sinR - sinY * sinP * cosR);
	camera_up_.set_y(cosP * cosR);
	camera_up_.set_z(-sinY * sinR - sinP * cosR * -cosY);

	// right vector
	camera_right_ = camera_forward_.CrossProduct(camera_up_);
	//std::cout << camera_position_.x() << "\n";

	//std::to_string(camera_position_.x())


}

void Camera::HandleInput(float dt, gef::Keyboard* input, gef::InputManager* in_manager, float width, float height/*, float x,float y*/)
{

	gef::Keyboard* keyboardManager = input;
	// forwards movement
	if (keyboardManager->IsKeyDown(gef::Keyboard::KC_W)) {
		//camera_position_.add(camera_forward_, camSpeed_ * dt);
		camera_position_ += (camera_forward_ * camSpeed_ * dt);
	}
	// backwards movement
	if (keyboardManager->IsKeyDown(gef::Keyboard::KC_S)) {
		camera_position_ += (camera_forward_ * -camSpeed_ * dt);
	}


	// left movement
	if (keyboardManager->IsKeyDown(gef::Keyboard::KC_A)) {
		camera_position_ += (camera_right_ * -camSpeed_ * dt);

	}



	// right movement
	if (keyboardManager->IsKeyDown(gef::Keyboard::KC_D)) {
		camera_position_ += (camera_right_ * camSpeed_ * dt);
	}


	// up movement
	if (keyboardManager->IsKeyDown(gef::Keyboard::KC_R)) {
		camera_position_ += (camera_up_ * camSpeed_ * dt);
	}
	// down movement
	if (keyboardManager->IsKeyDown(gef::Keyboard::KC_F)) {
		camera_position_ += (camera_up_ * -camSpeed_ * dt);
	}
	// x-axis rotation
	if (keyboardManager->IsKeyDown(gef::Keyboard::KC_U)) {
		pitch_ += rotSpeed_ * dt;
	}
	if (keyboardManager->IsKeyDown(gef::Keyboard::KC_J)) {
		pitch_ -= rotSpeed_ * dt;
	}
	// y-axis rotation
	if (keyboardManager->IsKeyDown(gef::Keyboard::KC_Y)) {
		yaw_ += rotSpeed_ * dt;
	}
	if (keyboardManager->IsKeyDown(gef::Keyboard::KC_H)) {
		yaw_ -= rotSpeed_ * dt;
	}
	// z-axis rotation
	if (keyboardManager->IsKeyDown(gef::Keyboard::KC_T)) {
		roll_ += rotSpeed_ * dt;
	}
	if (keyboardManager->IsKeyDown(gef::Keyboard::KC_G)) {
		roll_ -= rotSpeed_ * dt;
	}





	const gef::TouchInputManager* touch_input = in_manager->touch_manager();
	mouse_x_ = touch_input->mouse_position().x;
	mouse_y_ = touch_input->mouse_position().y;


	ShowCursor(false);
	mouse_x_ = touch_input->mouse_rel().x();
	mouse_y_ = touch_input->mouse_rel().y();
	mousePrison_[0] = (width / 2);
	mousePrison_[1] = (height / 2);

	yaw_ += (mouse_x_) * 0.08f;
	pitch_ += (mouse_y_) * -0.08f;
	//mouse_y_ = touch->position.y;


	// keep the mouse in the center of the window
	/*mousePrison_[0] = (width / 2);
	mousePrison_[1] = (height / 2);
	yaw_ -= -(mouse_x_ - mousePrison_[0]) * dt * 1.f;
	pitch_ += -(mouse_y_ - mousePrison_[1]) * dt * 1.f;*/

	//touch_input->mouse_position().x() = 0;
	//glutWarpPointer(mousePrison_[0], mousePrison_[1]);   //FIND A WAY TO DO THIS



}
