#include "scene_app.h"
#include <system/platform.h>
#include <graphics/sprite_renderer.h>
#include <graphics/font.h>
#include <system/debug_log.h>
#include <graphics/renderer_3d.h>
#include <graphics/mesh.h>
#include <maths/math_utils.h>
#include <input/sony_controller_input_manager.h>
#include <input/input_manager.h>
#include <input/keyboard.h>
#include <graphics/sprite.h>
#include "load_texture.h"



SceneApp::SceneApp(gef::Platform& platform) :
	Application(platform),
	sprite_renderer_(NULL),
	renderer_3d_(NULL),
	primitive_builder_(NULL),
	input_manager_(NULL),
	font_(NULL),
	world_(NULL),
	dt_multiplier_(NULL),
	active_touch_id_(-1)
{
}

void SceneApp::Init()
{
	// initialise input manager
	input_manager_ = gef::InputManager::Create(platform_);
	primitive_builder_ = new PrimitiveBuilder(platform_);
	sprite_renderer_ = gef::SpriteRenderer::Create(platform_);
	renderer_3d_ = gef::Renderer3D::Create(platform_);
	platform_.set_render_target_clear_colour(gef::Colour::Colour(0.5f, 0.5f, 0.5f, 1));

	InitFont();
	SetupLights();

	world_ = new b2World(b2Vec2(0.0f, 0.0f));

	// player
	player = new Player(world_, primitive_builder_);
	player->init(platform_);

	dt_multiplier_ = 60;

	// asteroids
	asteroid_destroyed_ = false;
	const char* player_asset_filename = "asteroid.scn";
	asteroid_model_ = LoadSceneAssets(platform_, player_asset_filename);
	asteroid_velocity_ = 200.06;
	asteroid_spawn_timer = 0;
	asteroid_spawn_timer_max = 70;

	score = 0;

}


////////    CLEAN UP    ////////   
void SceneApp::CleanUp()
{

	CleanUpFont();

	delete input_manager_;
	input_manager_ = NULL;

	delete primitive_builder_;
	primitive_builder_ = NULL;

	delete renderer_3d_;
	renderer_3d_ = NULL;

	delete sprite_renderer_;
	sprite_renderer_ = NULL;
	delete world_;
	world_ = NULL;
	score = 0;
}


////////    UPDATE    ////////       
bool SceneApp::Update(float frame_time)
{

	if (input_manager_){
		input_manager_->Update();
	}

	gef::Keyboard* keyManager = input_manager_->keyboard();

	for (size_t i = 0; i < asteroids_.size(); i++) {
		asteroids_[i].setMesh(GetMeshFromSceneAssets(asteroid_model_));
		asteroids_[i].update(frame_time, asteroid_velocity_, b2Vec2(0, 1));
	}

	UpdateTimers(frame_time);
	SpawnAsteroids(frame_time);


	if (!player->getKilled()) {
		player->handle_input(frame_time, keyManager, input_manager_, platform_);
		player->update(frame_time);
	}
	
	
	if (keyManager->IsKeyDown(gef::Keyboard::KC_ESCAPE)) {
		exit(0);
	}

	fps_ = 1.0f / frame_time;
	int velocityIteratons = 6;
	int32 positionIterations = 2;

	world_->Step(frame_time, velocityIteratons, positionIterations);
	CollisionDetection();
	DestroyObjects();

	return true;
}

////////    RENDER    ////////   
void SceneApp::Render()
{
	// projection
	float fov = gef::DegToRad(45.0f);
	float aspect_ratio = (float)platform_.width() / (float)platform_.height();
	gef::Matrix44 projection_matrix;
	projection_matrix = platform_.PerspectiveProjectionFov(fov, aspect_ratio, 0.1f, 100.0f);
	renderer_3d_->set_projection_matrix(projection_matrix);

	// view
	gef::Matrix44 view_matrix;
	view_matrix.LookAt(gef::Vector4(0, 70, 20), gef::Vector4(0, 0, -20), gef::Vector4(0, 1, 0));
	renderer_3d_->set_view_matrix(view_matrix);

	// draw 3d geometry
	renderer_3d_->Begin();
	if (!player->getKilled()) {
		player->render_player(renderer_3d_);
		}
		

		for (size_t i = 0; i < asteroids_.size(); i++) {
			asteroids_[i].render(renderer_3d_);
		}
	renderer_3d_->End();

	// start drawing sprites, but don't clear the frame buffer
	sprite_renderer_->Begin(false);
		DrawHUD();
	sprite_renderer_->End();
}


void SceneApp::moveUpMenu() {

}
void SceneApp::moveDownMenu() {

}
void SceneApp::UpdateMenuSate() {


}
void SceneApp::RenderMenuState() {

}
void SceneApp::stateUpdate() {
	switch (gameState_.getCurrentState()) {
	case State::Init:
		//UpdateInitState()
		//RenderInitState()
		break;
	case State::MENU:
		//UpdateMenuSate()
		//RenderMenuState()
		break;
	case State::CONTROLS:
		//UpdateControlsState()
		//RenderControlsState()
		break;
	case State::LEVEL:
		//UpdateLevelState()
		//RenderLevelState()
		break;
	case State::PAUSE:
		//UpdatePauseState()
		//RenderPauseState()
		break;
	case State::PAUSE_CONTROLS:
		//UpdatePauseControlsState()
		//RenderPauseControlsState()
		break;
	case State::DEATH:
		//UpdateDeathState()
		//RenderDeathState()
		break;
	case State::CREDITS:
		//UpdateCreditsState()
		//RenderCreditsState()
		break;

	}

}

////////    SPAWN ASTEROIDS    ////////   
void SceneApp::SpawnAsteroids(float frame_time)
{
	if (this->asteroid_spawn_timer >= this->asteroid_spawn_timer_max) {
		asteroids_.push_back(Asteroid(world_));
		this->asteroid_spawn_timer = 0; // Reset timer
	}
}

////////    COLLISION    ////////   
void SceneApp::CollisionDetection()
{
	b2Contact* contact = world_->GetContactList();
	int contact_count = world_->GetContactCount();

	for (int contact_num = 0; contact_num < contact_count; ++contact_num)
	{
		if (contact->IsTouching())
		{
			auto fA = contact->GetFixtureA();
			auto fB = contact->GetFixtureB();

			auto bA = fA->GetBody();
			auto bB = fB->GetBody();

			b2BodyUserData budA = bA->GetUserData();
			b2BodyUserData budB = bB->GetUserData();
			//gef::DebugOut("%i", budA.pointer , " %i: ", budB.pointer);
			if ((budA.pointer == 1 && budB.pointer == 5) || (budA.pointer == 5 && budB.pointer == 1)){
				if (budA.pointer == 1) {
					asteroids_to_kill.push_back(bA);
				}
				if (budB.pointer == 1) {
					asteroids_to_kill.push_back(bB);
				}
				if (budA.pointer == 5) {
					bullets_to_kill.push_back(bA);
				}
				if (budB.pointer == 5) {
					bullets_to_kill.push_back(bB);
				}
				asteroid_destroyed_ = true;

			}

			if ((budA.pointer == 1 && budB.pointer == 2) || (budA.pointer == 2 && budB.pointer == 1)) {
				if (budA.pointer == 1) {
					asteroids_to_kill.push_back(bA);
				}
				if (budB.pointer == 1) {
					asteroids_to_kill.push_back(bB);
				}
				asteroid_destroyed_ = true;
				player->setKilled(true);
			}



		}
		// Get next contact point
		contact = contact->GetNext();
	}
}


////////    DESTROY ASTEROIDS    ////////   
void SceneApp::DestroyObjects()
{
	for (int i = 0; i < asteroids_to_kill.size(); i++) {
		for (int j = 0; j < asteroids_.size(); j++) {
			if (asteroids_[j].getBody() == asteroids_to_kill[i]) {
				int health = asteroids_[j].getHP() - 1;
				asteroids_[j].setHP(health);

				gef::DebugOut("kill %i %i %i\n", asteroids_[j].getHP(), health, j);

				if (asteroids_[j].getHP() <= 0) {
					score += 10;
					asteroids_[j].getBody()->GetWorld()->DestroyBody(asteroids_[j].getBody());
					asteroids_.erase(asteroids_.begin() + j);
				}
			}
		}
	}
	asteroids_to_kill.clear();

	/*for (int i = 0; i < bullets_to_kill.size(); i++) {
		for (auto& bullet : player->getBullets()) {
			if (bullet.getBody() == bullets_to_kill[i]) {
				gef::DebugOut("ciao\n");
				player->getBullets()[i].getBody()->GetWorld()->DestroyBody(player->getBullets()[i].getBody());
				player->getBullets().erase(player->getBullets().begin() + i);
			}
		}
	}
	bullets_to_kill.clear();*/
	//for (int i = 0; i < bullets_to_kill.size(); i++) {
	//	for (int j = 0; j < player->getBullets().size(); j++) {
	//		if (player->getBullets()[j].getBody() == bullets_to_kill[i]) {
	//			player->getBullets()[j].getBody()->GetWorld()->DestroyBody(player->getBullets()[j].getBody());
	//			player->getBullets().erase(player->getBullets().begin() + j);
	//		}
	//	}
	//}
	//bullets_to_kill.clear();

}


////////    FONT    ////////   
void SceneApp::InitFont()
{
	font_ = new gef::Font(platform_);
	font_->Load("comic_sans");

	for (int i = 0; i < 3; i++) {
		Menu[i] = new gef::Font(platform_);
		Menu[i]->Load("spacemono");
	}
	for (int i = 0; i < 4; i++) {
		Pause[i] = new gef::Font(platform_);
		Pause[i]->Load("spacemono");
	}
}
void SceneApp::CleanUpFont()
{
	delete font_;
	font_ = NULL;
}


////////    HUD    ////////   
void SceneApp::DrawHUD()
{
	if (font_)
	{
		// display frame rate
		font_->RenderText(sprite_renderer_, gef::Vector4(850.0f, 510.0f, -0.9f), 1.0f, 0xffffffff, gef::TJ_LEFT, "FPS: %.1f", fps_);
		font_->RenderText(sprite_renderer_, gef::Vector4(50.0f, 10.0f, -0.9f), 1.0f, 0xffffffff, gef::TJ_LEFT, "SCORE: %i", score);
	}
}


////////    LIGHTS    ////////   
void SceneApp::SetupLights()
{
	// grab the data for the default shader used for rendering 3D geometry
	gef::Default3DShaderData& default_shader_data = renderer_3d_->default_shader_data();

	// set the ambient light
	default_shader_data.set_ambient_light_colour(gef::Colour(0.25f, 0.25f, 0.25f, 1.0f));

	// add a point light that is almost white, but with a blue tinge
	// the position of the light is set far away so it acts light a directional light
	gef::PointLight default_point_light;
	default_point_light.set_colour(gef::Colour(0.7f, 0.7f, 1.0f, 1.0f));
	default_point_light.set_position(gef::Vector4(-500.0f, 400.0f, 700.0f));
	default_shader_data.AddPointLight(default_point_light);
}


////////    UPDATE TIMERS    ////////   
void SceneApp::UpdateTimers(const float& dt) {

	// Enemy spawn 
	if (asteroid_spawn_timer < asteroid_spawn_timer_max)
		asteroid_spawn_timer += 1.f * dt * dt_multiplier_;
}



