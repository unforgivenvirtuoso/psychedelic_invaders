#pragma once


#include <engine.h>
#include "engine/entities/shapes/enemy.h"
#include "engine/entities/shapes/enemy2.h"
#include "engine/entities/shapes/enemy_bullet.h"
#include "engine/utils/timer.h"
#include "ballistic.h"
#include "engine/entities/bounding_box.h"
#include "billboard.h"
#include "cross_fade.h"
#include "alpha_sphere.h"




class enemy;



class example_layer : public engine::layer
{
public:
    example_layer();
	~example_layer();

	bool m_startscreen;
	bool powerup = false;
	bool shoot;
	int score = 0;
	int lives = 3;
	bool easy;
	float seconds = 0.f;
	float rot = 0.f;

	glm::vec3 pos;
	

	//letting fate choose colours seems to add a cool effect in line with game theme :))
	float r = ((int)rand() % 10) + 0.001;
	
	
	

    void on_update(const engine::timestep& time_step) override;
    void on_render() override; 
    void on_event(engine::event& event) override;

private:
	//void check_bounce();

	//collision boxes
	engine::bounding_box				m_camera_box;
	engine::bounding_box				m_powerup_box;
	engine::bounding_box				m_ball_box;
	engine::bounding_box				m_boss_box;
	engine::bounding_box				m_enemy_box;
	engine::bounding_box				m_enemy2_box;
	engine::bounding_box				m_enemy_bullet_box;
	engine::bounding_box				m_turret_box;

	// vector of the enemy boxes
	std::vector<engine::bounding_box>   m_enemy_boxes;
	std::vector<engine::ref<engine::game_object>>    m_enemies{};

	//objects
	engine::ref<engine::game_object>	m_camera{};

	engine::ref<engine::skybox>			m_skybox{};
	engine::ref<engine::game_object>	m_terrain{};

	engine::ref<engine::game_object>	m_cow{};
	engine::ref<engine::game_object>	m_tree{};
	engine::ref<engine::game_object>	m_ball{};
	engine::ref<engine::game_object>	m_mannequin{};

	engine::ref<engine::game_object>    m_turret{};
	engine::ref<engine::game_object>    m_boss{};
	engine::ref<engine::game_object>    m_powerup{};

	engine::ref<engine::game_object>    m_enemy{};
	engine::ref<engine::game_object>    m_enemy2{};
	engine::ref<engine::game_object>    m_enemy_bullet{};

	//material
	engine::ref<engine::material>		m_ballistic_material{};

	
	engine::ref<engine::material>		m_lightsource_material{};
	
	//light
	engine::PointLight					m_pointLight;
	uint32_t							num_point_lights = 1;
	engine::SpotLight					m_spotLight;
	uint32_t							num_spot_lights = 1;


	ballistic                           m_ballistic;

	

	

	engine::ref<engine::material>		m_material{};

	engine::DirectionalLight            m_directionalLight;

	std::vector<engine::ref<engine::game_object>>     m_game_objects{};

	engine::ref<engine::bullet_manager> m_physics_manager{};
	engine::ref<engine::audio_manager>  m_audio_manager{};
	//float								m_prev_sphere_y_vel = 0.f;
	engine::ref<engine::text_manager>	m_text_manager{};

    engine::orthographic_camera       m_2d_camera; 
    engine::perspective_camera        m_3d_camera;

	//special effects
	engine::ref<billboard>							m_billboard{};
	engine::ref<cross_fade>							m_cross_fade{};
	engine::ref<alpha_sphere>						m_alpha_sphere{};
};
