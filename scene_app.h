#ifndef _SCENE_APP_H
#define _SCENE_APP_H

#include <system/application.h>
#include <maths/vector2.h>
#include "primitive_builder.h"
#include <graphics/mesh_instance.h>
#include <input/input_manager.h>
#include <box2d/Box2D.h>
#include "game_object.h"
#include "Player.h"
#include "Asteroid.h"
#include "DynamicArray.h"
#include <vector>
#include"GameStates.h"
namespace gef
{
	class Platform;
	class SpriteRenderer;
	class Font;
	class InputManager;
	class Renderer3D;
}


class SceneApp : public gef::Application
{
public:
	SceneApp(gef::Platform& platform);
	void Init();
	void CleanUp();
	bool Update(float frame_time);
	void Render();
	void moveUpMenu();
	void moveDownMenu();
	void UpdateMenuSate();
	void RenderMenuState();
	void stateUpdate();
private:
	void CollisionDetection();
	void DestroyObjects();
	void InitFont();
	void CleanUpFont();
	void DrawHUD();
	void SetupLights();
	void SpawnAsteroids(float frame_time);
	void UpdateTimers(const float& dt);



	gef::SpriteRenderer* sprite_renderer_;
	gef::Font* font_;
	gef::InputManager* input_manager_;
	gef::Renderer3D* renderer_3d_;
	PrimitiveBuilder* primitive_builder_;
	b2World* world_;
	Int32 active_touch_id_;
	gef::Vector2 touch_position_;



	Player* player;
	gef::Scene* asteroid_model_;
	std::vector<b2Body*> asteroids_to_kill;
	std::vector<b2Body*> bullets_to_kill;
	std::vector<Asteroid> asteroids_;

	//GameState object 
	GameStates gameState_;

	//GameState Fonts 
	gef::Font* Menu[3];
	gef::Font* Pause[4];

	// sound
	int sfx_id_;
	int sfx_voice_id_;
	
	
	float dt_multiplier_;
	float fps_;
	float yPostion_;


	// asteroids
	float asteroid_spawn_timer, asteroid_spawn_timer_max;
	float asteroid_velocity_;
	bool asteroid_destroyed_;

	// score
	int score, scoreMultiplier;
	int scoreTime;


};

#endif // _SCENE_APP_H
