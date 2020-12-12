#include "example_layer.h"
#include "platform/opengl/gl_shader.h"

#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp>
#include "engine/events/key_event.h"
#include "engine/utils/track.h"
#include <random>
#include "engine/entities/shapes/enemy.h"
#include "engine/entities/shapes/enemy2.h"
#include "engine/entities/shapes/enemy_bullet.h"



#include <chrono>


example_layer::example_layer()
	:m_2d_camera(-1.6f, 1.6f, -0.9f, 0.9f),
	m_3d_camera((float)engine::application::window().width(), (float)engine::application::window().height())
	
{
	
    // Hide the mouse and lock it inside the window
    //engine::input::anchor_mouse(true);
    engine::application::window().hide_mouse_cursor();

	//start screen
	m_startscreen = true;

	// Initialise audio and play background music
	m_audio_manager = engine::audio_manager::instance();
	m_audio_manager->init();
	m_audio_manager->load_sound("assets/audio/bounce.wav", engine::sound_type::event, "bounce"); // Royalty free sound from freesound.org

	//play intro screen music
    //intro audio from https://samplefocus.com/samples/sci-fi-ambience-panned-loop
	//intro audio license https://samplefocus.com/license
	m_audio_manager->load_sound("assets/audio/intro_theme.wav", engine::sound_type::track, "intro_start");
	m_audio_manager->play("intro_start");

	//load game music
	//risen audio from https://visceracannon.bandcamp.com/album/hymns
	//RIP King Terry
	//1969 - 2018
	//risen audio license https://creativecommons.org/licenses/by/3.0/
	m_audio_manager->load_sound("assets/audio/risen_reduced_volume.mp3", engine::sound_type::track, "music");

	//load audio for powerup
	//powerup audio https://samplefocus.com/samples/8-bit-coin-fx
	//powerup audio license https://samplefocus.com/license
	m_audio_manager->load_sound("assets/audio/powerup_noise.wav", engine::sound_type::event, "powerup");
	
	//load audio for electric
	//electric audio https://www.freesoundeffects.com/free-sounds/electric-sounds-10032/ name of file on website is arc 1
	m_audio_manager->load_sound("assets/audio/electric.wav", engine::sound_type::event, "electric");

	//load audio for explosion
	//explosion audio https://www.freesoundeffects.com/free-sounds/war-and-battle-sound-effects-10042/ name of file on website is Explosion 3
	m_audio_manager->load_sound("assets/audio/explosion.wav", engine::sound_type::event, "explosion");

	//load audio for bullet
	//bullet audio https://www.zapsplat.com/music/laser-gun-single-shot-2/
	//bullet license https://www.zapsplat.com/license-type/standard-license/
	m_audio_manager->load_sound("assets/audio/bullet.mp3", engine::sound_type::event, "bullet");

	// Initialise the shaders, materials and lights
	auto mesh__material_shader = engine::renderer::shaders_library()->get("mesh_material");
	auto mesh_lighting_shader = engine::renderer::shaders_library()->get("mesh_lighting");
	auto text_shader = engine::renderer::shaders_library()->get("text_2D");
	auto animated_mesh_shader = engine::renderer::shaders_library()->get("animated_mesh");

	m_directionalLight.Color = glm::vec3(1.0f, 1.0f, 1.0f);
	m_directionalLight.AmbientIntensity = 0.25f;
	m_directionalLight.DiffuseIntensity = 0.6f;
	m_directionalLight.Direction = glm::normalize(glm::vec3(1.0f, -1.0f, 0.0f));

	//point light
	m_pointLight.Color = glm::vec3(1.f, 0.2f, 0.4f);
	m_pointLight.AmbientIntensity = 0.25f; //0.25
	m_pointLight.DiffuseIntensity = 0.6f; //0.6
	m_pointLight.Position = glm::vec3(0, 0, 0);

	//spot light
	m_spotLight.Color = glm::vec3(20.0f, 1.f, 1.f);
	m_spotLight.AmbientIntensity = 0.25f;
	m_spotLight.DiffuseIntensity = 0.6f;
	m_spotLight.Position = glm::vec3(0,17,-200.f);
	m_spotLight.Direction = glm::normalize(glm::vec3(0.0f, 0.f, 1.0f));
	m_spotLight.Cutoff = 0.1f;
	m_spotLight.Attenuation.Constant = 1.0f;
	m_spotLight.Attenuation.Linear = 0.1f;
	m_spotLight.Attenuation.Exp = 0.01f;
	


	// set color texture unit
	std::dynamic_pointer_cast<engine::gl_shader>(animated_mesh_shader)->bind();
	std::dynamic_pointer_cast<engine::gl_shader>(animated_mesh_shader)->set_uniform("gColorMap", 0);
	std::dynamic_pointer_cast<engine::gl_shader>(animated_mesh_shader)->set_uniform("lighting_on", true);
	m_directionalLight.submit(animated_mesh_shader);
	std::dynamic_pointer_cast<engine::gl_shader>(animated_mesh_shader)->set_uniform("gMatSpecularIntensity", 0.5f);
	std::dynamic_pointer_cast<engine::gl_shader>(animated_mesh_shader)->set_uniform("gSpecularPower", 5.f);
	std::dynamic_pointer_cast<engine::gl_shader>(animated_mesh_shader)->set_uniform("transparency", 1.0f);
	std::dynamic_pointer_cast<engine::gl_shader>(mesh__material_shader)->bind();
	std::dynamic_pointer_cast<engine::gl_shader>(mesh__material_shader)->set_uniform("lighting_on", true);
	m_directionalLight.submit(mesh__material_shader);
	std::dynamic_pointer_cast<engine::gl_shader>(mesh__material_shader)->set_uniform("gMatSpecularIntensity", 1.f);
	std::dynamic_pointer_cast<engine::gl_shader>(mesh__material_shader)->set_uniform("gSpecularPower", 10.f);
	std::dynamic_pointer_cast<engine::gl_shader>(mesh_lighting_shader)->bind();
	std::dynamic_pointer_cast<engine::gl_shader>(mesh_lighting_shader)->set_uniform("gColorMap", 0);
	std::dynamic_pointer_cast<engine::gl_shader>(mesh_lighting_shader)->set_uniform("lighting_on", true);
	m_directionalLight.submit(mesh_lighting_shader);
	std::dynamic_pointer_cast<engine::gl_shader>(mesh_lighting_shader)->set_uniform("gMatSpecularIntensity", 0.5f);
	std::dynamic_pointer_cast<engine::gl_shader>(mesh_lighting_shader)->set_uniform("gSpecularPower", 5.f);
	std::dynamic_pointer_cast<engine::gl_shader>(mesh_lighting_shader)->set_uniform("transparency", 1.0f);
	
	std::dynamic_pointer_cast<engine::gl_shader>(text_shader)->bind();
	std::dynamic_pointer_cast<engine::gl_shader>(text_shader)->set_uniform("projection",
		glm::ortho(0.f, (float)engine::application::window().width(), 0.f,
		(float)engine::application::window().height()));

	m_material = engine::material::create(1.0f, glm::vec3(1.0f, 0.1f, 0.07f),
		glm::vec3(1.0f, 0.1f, 0.07f), glm::vec3(0.5f, 0.5f, 0.5f), 1.0f);

	//spotlight material
	m_lightsource_material = engine::material::create(1.0f, m_spotLight.Color,
		m_spotLight.Color, m_spotLight.Color, 1.0f);

	//point light material
	m_ballistic_material = engine::material::create(1.0f, m_pointLight.Color,
		m_pointLight.Color, m_pointLight.Color, 1.0f);

	

	// Skybox texture from   https://wwwtyro.github.io/space-3d/  Randomised Seed was : x637rdnuxr4

	m_skybox = engine::skybox::create(50.f,
		{ engine::texture_2d::create("assets/textures/front.png", true),
		  engine::texture_2d::create("assets/textures/right.png", true),
		  engine::texture_2d::create("assets/textures/back.png", true),
		  engine::texture_2d::create("assets/textures/left.png", true),
		  engine::texture_2d::create("assets/textures/top.png", true),
		  engine::texture_2d::create("assets/textures/bottom.png", true)
		});
	

	engine::ref<engine::skinned_mesh> m_skinned_mesh = engine::skinned_mesh::create("assets/models/animated/mannequin/free3Dmodel.dae");
	m_skinned_mesh->LoadAnimationFile("assets/models/animated/mannequin/walking.dae");
	m_skinned_mesh->LoadAnimationFile("assets/models/animated/mannequin/idle.dae");
	m_skinned_mesh->LoadAnimationFile("assets/models/animated/mannequin/jump.dae");
	m_skinned_mesh->LoadAnimationFile("assets/models/animated/mannequin/standard_run.dae");
	m_skinned_mesh->switch_root_movement(false);

	engine::game_object_properties mannequin_props;
	mannequin_props.animated_mesh = m_skinned_mesh;
	mannequin_props.scale = glm::vec3(1.f/ glm::max(m_skinned_mesh->size().x, glm::max(m_skinned_mesh->size().y, m_skinned_mesh->size().z)));
	mannequin_props.position = glm::vec3(3.0f, 0.5f, -5.0f);
	mannequin_props.type = 0;
	mannequin_props.bounding_shape = m_skinned_mesh->size() / 2.f * mannequin_props.scale.x;
	m_mannequin = engine::game_object::create(mannequin_props);

	// Load the terrain texture and create a terrain mesh. Create a terrain object. Set its properties
	// terrain texture https://texturehaven.com/tex/?c=terrain&t=forrest_ground_01
	std::vector<engine::ref<engine::texture_2d>> terrain_textures = { engine::texture_2d::create("assets/textures/terrain.jpg", false) };
	engine::ref<engine::terrain> terrain_shape = engine::terrain::create(100.f, 0.5f, 100.f);
	engine::game_object_properties terrain_props;
	terrain_props.meshes = { terrain_shape->mesh() };
	terrain_props.textures = terrain_textures;
	terrain_props.is_static = true;
	terrain_props.type = 0;
	terrain_props.bounding_shape = glm::vec3(100.f, 0.5f, 100.f);
	terrain_props.restitution = 0.92f;
	m_terrain = engine::game_object::create(terrain_props);

	// boss model https://free3d.com/3d-model/sequin-jester-mask-v1--494203.html
	engine::ref <engine::model> boss_model = engine::model::create("assets/models/static/boss.obj");
	engine::game_object_properties boss_props;
	boss_props.meshes = boss_model->meshes();
	boss_props.textures = boss_model->textures();
	float boss_scale = 20.f / glm::max(boss_model->size().x, glm::max(boss_model->size().y, boss_model->size().z));
	boss_props.position = { 0,0,0 };
	boss_props.scale = glm::vec3(boss_scale);
	boss_props.bounding_shape = boss_model->size();
	m_boss = engine::game_object::create(boss_props);
	m_boss_box.set_box(10,10,10,
		glm::vec3(0,17.f, m_3d_camera.z - rot * 1.9));

	//player/camera model https://free3d.com/3d-model/star-fighter-657163.html
	/*intial idea was to use a model and have some of it in in view of the camera for aesthetics
	 but it didnt turn out in the way i hoped so i just made the model small to use it for the bounding box
	 and keep it out of view of the camera 
	*/
	engine::ref <engine::model> player_model = engine::model::create("assets/models/static/ship.obj");
	engine::game_object_properties player_props;
	player_props.meshes = player_model->meshes();
	player_props.textures = player_model->textures();
	float player_scale = 0.8f / glm::max(player_model->size().x, glm::max(player_model->size().y, player_model->size().z));
	player_props.position = { m_3d_camera.position()  };
	player_props.scale = glm::vec3(player_scale);
	player_props.bounding_shape = player_model->size();
	m_camera = engine::game_object::create(player_props);
	m_camera->set_offset(player_model->offset());
	m_camera_box.set_box(player_props.bounding_shape.x *  player_scale ,
		player_props.bounding_shape.y* player_scale,
		player_props.bounding_shape.z* player_scale,
		glm::vec3(0,0,0));

	


	//turret model https://free3d.com/3d-model/star-wars-imperial-turret-58616.html
	engine::ref <engine::model> turret_model = engine::model::create("assets/models/static/turret.obj");
	engine::game_object_properties turret_props;
	turret_props.meshes = turret_model->meshes();
	turret_props.textures = turret_model->textures();
	float turret_scale = 1.f / glm::max(turret_model->size().x, glm::max(turret_model->size().y, turret_model->size().z));
	turret_props.position = { -5.f, 2.f, 6.f };
	turret_props.scale = glm::vec3(turret_scale);
	turret_props.bounding_shape = turret_model->size();
	m_turret = engine::game_object::create(turret_props);
	m_turret_box.set_box(turret_props.bounding_shape.x* turret_scale,
		turret_props.bounding_shape.y* turret_scale,
		turret_props.bounding_shape.z* turret_scale,
		turret_props.position);

	//powerup model https://free3d.com/3d-model/laurel-wreath-with-lightning-bolt-v2--164822.html
	//powerup texture https://www.freepik.com/free-vector/overlapping-forms-background_10073459.htm#page=1&query=yellow&position=15
	engine::ref <engine::model> powerup_model = engine::model::create("assets/models/static/light_powerup.obj");
	engine::game_object_properties powerup_props;
	powerup_props.meshes = powerup_model->meshes();
	powerup_props.textures = powerup_model->textures();
	float powerup_scale = 1.f / glm::max(powerup_model->size().x, glm::max(powerup_model->size().y, powerup_model->size().z));
	powerup_props.position = { 7.f, 7.f, -35.f };
	powerup_props.scale = glm::vec3(powerup_scale);
	powerup_props.bounding_shape = powerup_model->size();
	m_powerup = engine::game_object::create(powerup_props);
	m_powerup->set_offset(powerup_model->offset());
	m_powerup_box.set_box(3,3,3,
		powerup_props.position);
	

	// Load the tree model. Create a tree object. Set its properties
	engine::ref <engine::model> tree_model = engine::model::create("assets/models/static/elm.3ds");
	engine::game_object_properties tree_props;
	tree_props.meshes = tree_model->meshes();
	tree_props.textures = tree_model->textures();
	float tree_scale = 3.f / glm::max(tree_model->size().x, glm::max(tree_model->size().y, tree_model->size().z));
	tree_props.position = { 4.f, 0.5f, -5.f };
	tree_props.bounding_shape = tree_model->size() / 2.f * tree_scale;
	tree_props.scale = glm::vec3(tree_scale);
	m_tree = engine::game_object::create(tree_props);

	engine::ref<engine::sphere> sphere_shape = engine::sphere::create(10, 20, 0.5f);
	engine::game_object_properties sphere_props;
	sphere_props.position = { 0.f, -20.f, -5.f };
	sphere_props.meshes = { sphere_shape->mesh() };
	sphere_props.type = 1;
	sphere_props.bounding_shape = glm::vec3(0.5f);
	sphere_props.restitution = 0.92f;
	sphere_props.mass = 1.f;
	m_ball = engine::game_object::create(sphere_props);

	//initialise ballistic to sphere object
	m_ballistic.initialise(engine::game_object::create(sphere_props));

	//create box for ballistic
	m_ball_box.set_box(sphere_props.bounding_shape.x,
		sphere_props.bounding_shape.y,
		sphere_props.bounding_shape.z,
		sphere_props.position);
	
	for (int i = 0; i <= 10; i++) {
		//enemy vertices
			std::vector<glm::vec3> enemy_vertices;
		enemy_vertices.push_back(glm::vec3(2.f, 2.f, 1.f));//0
		enemy_vertices.push_back(glm::vec3(0.f, 0.f, 2.f));//1
		enemy_vertices.push_back(glm::vec3(-7.f, 1.f, -2.f)); //2
		enemy_vertices.push_back(glm::vec3(1.f, 0.f, -2.f)); //3
		// create enemy
		engine::ref<engine::enemy> enemy_shape =
			engine::enemy::create(enemy_vertices);
		engine::game_object_properties enemy_props;


		// shipbase texture for enemy primitive https://3dtextures.me/2020/10/07/sci-fi-wall-007/
		std::vector<engine::ref<engine::texture_2d>> enemy_textures =
		{ engine::texture_2d::create("assets/textures/shipbase.jpg", false)

		};

		enemy_props.textures = enemy_textures;

		// enemy_transform = glm::translate(enemy_transform, glm::vec3((40.f - i * 5.f), (1.f + i * 1.1f), (i - 3.f * 10.f)));
		enemy_props.position = { 30.f - i * 7.f, 1.f + i * 3.f, i - 5.f * 10.f };
		enemy_props.rotation_amount = -300;
		enemy_props.rotation_axis = glm::vec3(0.f, 1.f, 0.f);

		enemy_props.meshes = { enemy_shape->mesh() };

		auto tmp_m_enemy = engine::game_object::create(enemy_props);
		m_enemies.push_back(tmp_m_enemy);
	}
	for (int i = 0; i <= 10; i++) {
		engine::bounding_box tmp_enemy_box;
		tmp_enemy_box.set_box(10, 2, 10, glm::vec3(0,0,0));
		m_enemy_boxes.push_back(tmp_enemy_box);
	}
	//enemy2
	std::vector<glm::vec3> enemy2_vertices;
	enemy2_vertices.push_back(glm::vec3(0.f, 2.f, 12.f));//0
	enemy2_vertices.push_back(glm::vec3(2.f, 0.f, 2.f));//1
	enemy2_vertices.push_back(glm::vec3(-2.f, 2.f, -2.f)); //2
	enemy2_vertices.push_back(glm::vec3(8.f, 0.f, -2.f)); //3

	engine::ref<engine::enemy> enemy2_shape =
		engine::enemy::create(enemy2_vertices);
	engine::game_object_properties enemy2_props;


	// shipbase texture for enemy2 primitive https://www.freepik.com/free-vector/psychedelic-optical-illusion-wallpaper_8622320.htm#page=1&query=psychedelic%20texture&position=24
	std::vector<engine::ref<engine::texture_2d>> enemy2_textures =
	{ engine::texture_2d::create("assets/textures/enemy2.jpg", false)

	};

	enemy2_props.textures = enemy2_textures;


	enemy2_props.position = { 3.f, 5.f, -20.f };
	enemy2_props.meshes = { enemy2_shape->mesh() };
	m_enemy2 = engine::game_object::create(enemy2_props);
	m_enemy2_box.set_box(enemy2_props.bounding_shape.x,
		enemy2_props.bounding_shape.y,
		enemy2_props.bounding_shape.z,
		enemy2_props.position);
	


	//enemy bullet
	std::vector<glm::vec3> enemy_bullet_vertices;
	enemy_bullet_vertices.push_back(glm::vec3(9.f, 2.f, 1.f));
	enemy_bullet_vertices.push_back(glm::vec3(5.f, 3.f, 1.f));
	enemy_bullet_vertices.push_back(glm::vec3(4.f, 4.f, 1.f));
	enemy_bullet_vertices.push_back(glm::vec3(8.f, -1.f, 1.f));
	enemy_bullet_vertices.push_back(glm::vec3(7.f, 3.f, 1.f));
	
	
 

	engine::ref<engine::enemy> enemy_bullet_shape = engine::enemy::create(enemy_bullet_vertices);
	engine::game_object_properties enemy_bullet_props;


	// enemy bullet texture https://www.freepik.com/free-photo/abstract-saturated-psychedelic-vivid-background_5133846.htm#page=1&query=psychedelic&position=5
	std::vector<engine::ref<engine::texture_2d>> enemy_bullet_textures =
	{ engine::texture_2d::create("assets/textures/enemy_bullet.jpg", false)

	};

	enemy_bullet_props.textures = enemy_bullet_textures;


	enemy_bullet_props.position = { 3.f, 0.5f, -20.f };
	enemy_bullet_props.meshes = { enemy_bullet_shape->mesh() };
	m_enemy_bullet = engine::game_object::create(enemy_bullet_props);



	m_game_objects.push_back(m_terrain);
	m_game_objects.push_back(m_ball);
	m_game_objects.push_back(m_tree);
	m_game_objects.push_back(m_powerup);
	//m_game_objects.push_back(m_enemy);
	m_game_objects.push_back(m_enemy2);
	m_game_objects.push_back(m_enemy_bullet);
	//m_game_objects.push_back(m_turret);
	m_game_objects.push_back(m_boss);
	m_game_objects.push_back(m_camera);
	m_game_objects.push_back(m_powerup);
	
	//create billboard
	m_billboard = billboard::create("assets/textures/Explosion.tga", 4, 5, 16);

	//create crossfade
	m_cross_fade = cross_fade::create("assets/textures/yellow.bmp", 5.0f, 1.6f, 0.9f);

	//create alpha sphere
	m_alpha_sphere = alpha_sphere::create(glm::vec3(1.f, 0.f, 0.8f), glm::vec3(1.f, 0.f, 0.2f), true, 0.5f, 2.0f);

	m_physics_manager = engine::bullet_manager::create(m_game_objects);

	m_text_manager = engine::text_manager::create();

	m_skinned_mesh->switch_animation(1);
}

example_layer::~example_layer() {}

void example_layer::on_update(const engine::timestep& time_step) 
{
	//state for in game 
	if (!m_startscreen) {

		// variable for camera position
		 pos = m_3d_camera.position();

		//updates ball box
		m_ball_box.on_update(m_ballistic.object()->position());

		//boss box update
		m_boss_box.on_update(m_boss->position());

		//update point light to follow ballistic
		m_pointLight.Position = m_ballistic.object()->position();

		//update spotlight to follow boss doesnt work as intended so commented out the update for its position
		//m_spotLight.Position = m_boss->position();
		
		//billboard update
		m_billboard->on_update(time_step);

		//update crossfade
		m_cross_fade->on_update(time_step);

		//update alpha sphere
		m_alpha_sphere->on_update(time_step);

		//updates turrets
		m_turret_box.on_update(m_turret->position());

		//updates enemy
		for (int i = 0; i < m_enemies.size(); i++) {
			m_enemy_boxes[i].on_update(m_enemies[i]->position());
		}

		//rotating enemy
		for (auto e : m_enemies) {

			e->set_rotation_amount(rot);
			e->set_rotation_axis(glm::vec3(1, 0, 0));
			
			
		}
		//updates camera
		m_3d_camera.on_update(time_step);

		//updates time 
		seconds += time_step;

		//update score
		score += time_step;
		//update lives
		lives += time_step;
		//updates rotation speed but also used for incrementing movement speed
		rot += time_step * 3.f;

		//camera box update
		m_camera_box.on_update(pos);

		
		
		//collision for player and powerup but crossfade doesnt work in m_3d_camera :((
		if (m_powerup_box.collision(m_camera_box)) {
			powerup = true;
			m_cross_fade->activate();
			m_audio_manager->play("powerup");
			
		}

		//updates ballistic as well as bool for powerup active
		if (powerup) {
			m_ballistic.on_update(time_step * 5.f);
		}
		else {
			m_ballistic.on_update(time_step);
			
		}

		//checking collision with ballistic and vector of enemies
		for (int i = 0; i < 10; i++) {
			if (m_ball_box.collision(m_enemy_boxes[i])) {
				score += 200;

				//"despawning" ball at collision
				m_ballistic.object()->set_position(glm::vec3(-100, -400, 0));

				//explosion at position of enemy
				m_billboard->activate(m_enemies[i]->position(), 20.f, 20.f);

				//alpha sphere at enemy position
				m_alpha_sphere->activate(5.f, (m_enemies[i]->position()));

				// play electric audio
				m_audio_manager->play("electric");

				// play explosion audio
				m_audio_manager->play("explosion");

				//"despawning" enemies at collision with ballistic
				m_enemies[i]->set_position(glm::vec3(-100,-100,-100));
			}
		}
		//collision for player and enemy
		for (int i = 0; i < 10; i++) {
			if (m_camera_box.collision(m_enemy_boxes[i])) {
				//reduce score and lives
				score -= 100;
				lives -= 1;
		
				//explosion at position of enemy
				m_billboard->activate(m_enemies[i]->position(), 20.f, 20.f);

				// play explosion audio
				m_audio_manager->play("explosion");

				//"despawning" enemies at collision with ballistic
				m_enemies[i]->set_position(glm::vec3(-100, -100, -100));
			}
		}

		//ends game and switches back to introscreen but has a bug :((
		if (lives < 1) {
			m_startscreen = true;
			
		}
		
	}

	//state for intro screen
	if (m_startscreen) {
		//updates intro
		m_2d_camera.update_intro(time_step);

		//incrementing time_step again to have objects rotate in start screen
		rot += time_step;
	}

	

	m_physics_manager->dynamics_world_update(m_game_objects, double(time_step));

	m_mannequin->animated_mesh()->on_update(time_step);

	//check_bounce();
} 

void example_layer::on_render()
{
	engine::render_command::clear_color({ 0.2f, 0.3f, 0.3f, 1.0f });
	engine::render_command::clear();


	//const auto textured_shader = engine::renderer::shaders_library()->get("mesh_static");
	//engine::renderer::begin_scene(m_3d_camera, textured_shader);
	
		const auto textured_lighting_shader = engine::renderer::shaders_library()->get("mesh_lighting");
		engine::renderer::begin_scene(m_3d_camera, textured_lighting_shader);



		// Set up some of the scene's parameters in the shader
		std::dynamic_pointer_cast<engine::gl_shader>(textured_lighting_shader)->set_uniform("gEyeWorldPos", m_3d_camera.position());

		glm::mat4 skybox_tranform(1.0f);
		skybox_tranform = glm::translate(skybox_tranform, m_3d_camera.position());
		for (const auto& texture : m_skybox->textures())
		{
			texture->bind();
		}

		//loop for generating terrain
		for (int i = 1; i < 10 ; i++) {
			glm::mat4 terrain_transform(1.0f);
			terrain_transform = glm::translate(terrain_transform, glm::vec3(0.f, 0.0f, 120.f - i * 200.f));
			terrain_transform = glm::scale(terrain_transform, glm::vec3(1.0f));
			engine::renderer::submit(textured_lighting_shader, terrain_transform, m_terrain);
		}

		engine::renderer::submit(textured_lighting_shader, m_skybox, skybox_tranform);

		
		std::dynamic_pointer_cast<engine::gl_shader>(textured_lighting_shader)->set_uniform("gNumSpotLights", (int)num_spot_lights);
		m_spotLight.submit(textured_lighting_shader, 0);
		
		std::dynamic_pointer_cast<engine::gl_shader>(textured_lighting_shader)->set_uniform("gNumPointLights", (int)num_point_lights);
		m_pointLight.submit(textured_lighting_shader, 0);

		//adding some foiliage to intro screen and some objects
		if (m_startscreen) {
			for (int i = 1; i < 70; i++) {
				glm::mat4 tree_transform(1.0f);
				tree_transform = glm::translate(tree_transform, glm::vec3(-20.f + i *15, 0.f, -5.f));
				tree_transform = glm::rotate(tree_transform, m_tree->rotation_amount(), m_tree->rotation_axis());
				tree_transform = glm::scale(tree_transform, glm::vec3(0.5f));
				engine::renderer::submit(textured_lighting_shader, tree_transform, m_tree);

				glm::mat4 tree_transform1(1.0f);
				tree_transform1 = glm::translate(tree_transform1, glm::vec3(-20.f + i * 10, 0.5f, 0.f ));
				tree_transform1 = glm::rotate(tree_transform1, m_tree->rotation_amount(), m_tree->rotation_axis());
				tree_transform1 = glm::scale(tree_transform1, glm::vec3(1.f));
				engine::renderer::submit(textured_lighting_shader, tree_transform1, m_tree);

				
			}
			glm::mat4 powerup_transform(1.0f);
				powerup_transform = glm::translate(powerup_transform, glm::vec3(-5.f + rot, 3.f, 7.f));
				powerup_transform = glm::rotate(powerup_transform, rot, glm::vec3(0.25f, 0.5f, 1.0f));
				powerup_transform = glm::scale(powerup_transform, glm::vec3(0.5f));
				engine::renderer::submit(textured_lighting_shader, powerup_transform, m_powerup);

				glm::mat4 enemy2_transform(0.5f);
				enemy2_transform = glm::translate(enemy2_transform, glm::vec3((0.f), (8.f), (5)));
				enemy2_transform = glm::rotate(enemy2_transform, -300.f, glm::vec3(0.f, 1.f, 0.f));
				enemy2_transform = glm::rotate(enemy2_transform, rot + 1.5f, glm::vec3(0.f, 1.f, 0.f));
				//uncomment lines below to see a combination of rotation on all axis 
				//enemy_transform = glm::rotate(enemy_transform, rot, glm::vec3(0.f, 1.f, 0.f));
				//enemy_transform = glm::rotate(enemy_transform, rot, glm::vec3(0.f, 0.f, 1.f));
				enemy2_transform = glm::scale(enemy2_transform, glm::vec3(0.5f));
				engine::renderer::submit(textured_lighting_shader, enemy2_transform, m_enemy2);

			//having enemy bullets go pass camera which i think looks cool :))
			//pls wait in intro screen to see this effect to go past camera
			for (int i = 1; i < 11; i++) {

					glm::mat4 enemy_bullet_transform(1.0f);

					enemy_bullet_transform = glm::translate(enemy_bullet_transform, glm::vec3(-10.f + i * 3.f, 5.f, -10.f + rot * 1.5f));
					enemy_bullet_transform = glm::rotate(enemy_bullet_transform, rot + i * 3, glm::vec3(1.f, 0.5f, 0.25f));
					enemy_bullet_transform = glm::rotate(enemy_bullet_transform, rot + i * 2.3f, glm::vec3(0.25f, 1.f, 0.5f));
					enemy_bullet_transform = glm::rotate(enemy_bullet_transform, rot + i * 4.2f, glm::vec3(0.5f, 0.25f, 1.f));
					enemy_bullet_transform = glm::scale(enemy_bullet_transform, glm::vec3(0.25f));
					engine::renderer::submit(textured_lighting_shader, enemy_bullet_transform, m_enemy_bullet);

				}
		}
		

		if (!m_startscreen) {
		
			//some trees for scenary 
			for (int i = 1; i < 16; i++) {

				glm::mat4 tree_transform(1.0f);
				tree_transform = glm::translate(tree_transform, glm::vec3((3.f + i * 15.f), 0.5f, (12.f - i * 10.f)));
				tree_transform = glm::rotate(tree_transform, m_tree->rotation_amount(), m_tree->rotation_axis());
				tree_transform = glm::scale(tree_transform, glm::vec3(0.5f));
				engine::renderer::submit(textured_lighting_shader, tree_transform, m_tree);

				glm::mat4 tree_transform1(1.0f);
				tree_transform1 = glm::translate(tree_transform1, glm::vec3((3.f - i * 15.f), 0.5f, (12.f - i * 10.f)));
				tree_transform1 = glm::rotate(tree_transform1, m_tree->rotation_amount(), m_tree->rotation_axis());
				tree_transform1 = glm::scale(tree_transform1, glm::vec3(1.f));
				engine::renderer::submit(textured_lighting_shader, tree_transform1, m_tree);

			}

			//rendering vector of enemies and the collision boxes
			for (auto e : m_enemies) {
					engine::renderer::submit(textured_lighting_shader, e);
				}
				
			for (auto b : m_enemy_boxes) {
					b.on_render(2.5f, 0.f, 0.f, textured_lighting_shader);
				}

			//loop for adding turrets to the scene
			for (int i = 1; i < 6; i++) {

				
				//rotate the turrets towards the camera as it passes
				glm::mat4 turret_transform(1.0f);
				glm::vec3 p = glm::vec3(-15.f, 0.5f, -30.f - i * 10);
				glm::vec3 c = m_3d_camera.position();
				glm::vec3 v = c - p;
				float theta = atan2(v.x, v.z);
				turret_transform = glm::translate(turret_transform, p);
				turret_transform = glm::rotate(turret_transform, theta, glm::vec3(0.f, 1.0f, 0.f));
				turret_transform = glm::scale(turret_transform, glm::vec3(0.1f));
				engine::renderer::submit(textured_lighting_shader, turret_transform, m_turret);
				
			}

			
			
			//render boss
			if (score > 700) {
				glm::mat4 boss_transform(1.0f);
				boss_transform = glm::translate(boss_transform, glm::vec3(0.f, 3.f, m_3d_camera.z - rot  * 1.9));
				boss_transform = glm::rotate(boss_transform, 300.f, glm::vec3(1.f, 0.f, 0.f));
				boss_transform = glm::scale(boss_transform, glm::vec3(0.75f));
				engine::renderer::submit(textured_lighting_shader, boss_transform, m_boss);
				m_boss_box.on_render(2.5f, 0.f, 0.f, textured_lighting_shader);

			}

			//render camera box as well as mesh its attached to
			m_camera_box.on_render(2.5f, 0.f, 0.f, textured_lighting_shader);

			glm::mat4 player_transform(0.8f);
			player_transform = glm::translate(player_transform, m_3d_camera.position());
			player_transform = glm::rotate(player_transform, 0.f, glm::vec3(0.f, 0.f, 0.f));
			player_transform = glm::scale(player_transform, glm::vec3(0.8f));
			engine::renderer::submit(textured_lighting_shader, player_transform, m_camera);

			// render powerup 
			m_powerup_box.on_render(2.5f, 0.f, 0.f, textured_lighting_shader);
			glm::mat4 powerup_transform(1.0f);
			powerup_transform = glm::translate(powerup_transform, glm::vec3(7.f, 9.f, -35.f));
			powerup_transform = glm::rotate(powerup_transform, rot , glm::vec3(0.25f, 0.5f, 1.0f));
			powerup_transform = glm::scale(powerup_transform, glm::vec3(0.5f));
			engine::renderer::submit(textured_lighting_shader, powerup_transform, m_powerup);


			//loop for rendering enemy bullets that boss shoots
			if (score > 700) {
				for (int i = 1; i < 11; i++) {
					
						glm::mat4 enemy_bullet_transform(1.0f);
						
						enemy_bullet_transform = glm::translate(enemy_bullet_transform, glm::vec3(-20.f + i * 5, 15, m_3d_camera.z - rot * 1.9));
						enemy_bullet_transform = glm::rotate(enemy_bullet_transform, rot + i * 3, glm::vec3(1.f, 0.5f, 0.25f));
						enemy_bullet_transform = glm::rotate(enemy_bullet_transform, rot + i * 2.3f, glm::vec3(0.25f, 1.f, 0.5f));
						enemy_bullet_transform = glm::rotate(enemy_bullet_transform, rot + i * 4.2f, glm::vec3(0.5f, 0.25f, 1.f));
						enemy_bullet_transform = glm::scale(enemy_bullet_transform, glm::vec3(0.25f));
						engine::renderer::submit(textured_lighting_shader, enemy_bullet_transform, m_enemy_bullet);
					
				}
			}

			//render enemy2
			glm::mat4 enemy2_transform(0.5f);
			enemy2_transform = glm::translate(enemy2_transform, glm::vec3(-7.f, 10.f, -15.f));
			enemy2_transform = glm::rotate(enemy2_transform, -300.f, glm::vec3(0.f, 1.f, 0.f));
			enemy2_transform = glm::rotate(enemy2_transform, rot + 1.5f, glm::vec3(0.25f, 1.f, 0.5f));
			enemy2_transform = glm::scale(enemy2_transform, glm::vec3(0.5f));
			engine::renderer::submit(textured_lighting_shader, enemy2_transform, m_enemy2);
			m_enemy2_box.on_render(2.5f, 0.f, 0.f, textured_lighting_shader);
			
		}

	engine::renderer::end_scene();

	if (!m_startscreen) {
		// Set up material shader. (does not render textures, renders materials instead)
		const auto material_shader = engine::renderer::shaders_library()->get("mesh_material");
		engine::renderer::begin_scene(m_3d_camera, material_shader);

		m_material->submit(material_shader);
		std::dynamic_pointer_cast<engine::gl_shader>(material_shader)->set_uniform("gEyeWorldPos", m_3d_camera.position());

		// render alpha sphere
		m_alpha_sphere->on_render(material_shader);

		std::dynamic_pointer_cast<engine::gl_shader>(material_shader)->set_uniform("gNumSpotLights", (int)num_spot_lights);
		m_spotLight.submit(material_shader, 0);


		//std::dynamic_pointer_cast<engine::gl_shader>(material_shader)->set_uniform("lighting_on", false);

		std::dynamic_pointer_cast<engine::gl_shader>(material_shader)->set_uniform("lighting_on", true);

		m_lightsource_material->submit(material_shader);
		// using lights for the eyes of the boss
		if (score > 700) {
			engine::renderer::submit(material_shader, m_ball->meshes().at(0), glm::translate(glm::mat4(1.f), glm::vec3(2.5f, 17.f, m_3d_camera.z - rot * 1.9)));
			engine::renderer::submit(material_shader, m_ball->meshes().at(0), glm::translate(glm::mat4(1.f), glm::vec3(-2.5f, 17.f, m_3d_camera.z - rot * 1.9)));
		}

		
			//point light for ballistic unsure how to make the light glow move with ballistic :((
			std::dynamic_pointer_cast<engine::gl_shader>(material_shader)->set_uniform("gNumPointLights", (int)num_point_lights);
			m_pointLight.submit(material_shader, 0);

			std::dynamic_pointer_cast<engine::gl_shader>(material_shader)->set_uniform("lighting_on", true);

			m_ballistic_material->submit(material_shader);
			m_ballistic.on_render(material_shader);
			engine::renderer::submit(material_shader, m_ball);
			m_ballistic.object();
			m_ball_box.on_render(2.5f, 0.f, 0.f, textured_lighting_shader);
			engine::renderer::end_scene();
		
	}
	glm::mat4 aniTransform = glm::mat4(1.0f);

	engine::renderer::end_scene();

	//render special effects

	engine::renderer::begin_scene(m_3d_camera, textured_lighting_shader);
	m_billboard->on_render(m_3d_camera, textured_lighting_shader);

	//unsure why cross fade isnt working attempted to make it for m_3d_camera but wasnt able to but it seems to work fine with m_2d_camera
	engine::renderer::begin_scene(m_3d_camera, textured_lighting_shader);
	m_cross_fade->on_render(m_3d_camera, textured_lighting_shader);



	engine::renderer::end_scene();
	

	//letting fate choose colours seems to add a cool effect in line with game theme :))
	float r = ((int)rand() % 10) + 0.001;

	// Render text
	if (m_startscreen) {

		const auto text_shader = engine::renderer::shaders_library()->get("text_2D");

		//controls
		m_text_manager->render_text(text_shader, "W- Up",  (float)engine::application::window().width() / 12, (float)engine::application::window().height() / 2, 0.5f, glm::vec4(r, 0.5f, 0.3f, 1.f));
		m_text_manager->render_text(text_shader, "A- Left", (float)engine::application::window().width() / 6, (float)engine::application::window().height() / 2, 0.5f, glm::vec4(r, r, 0.3f, 1.f));
		m_text_manager->render_text(text_shader, "S- Down", (float)engine::application::window().width() / 4, (float)engine::application::window().height() / 2, 0.5f, glm::vec4(0.75f, r, r, 1.f));
		m_text_manager->render_text(text_shader, "D- Right", (float)engine::application::window().width() / 3, (float)engine::application::window().height() / 2, 0.5f, glm::vec4(0.3f, 0.5f , r, 1.f));
		m_text_manager->render_text(text_shader, "Mouse- Turn", (float)engine::application::window().width() / 2.3 , (float)engine::application::window().height() / 2, 0.5f, glm::vec4(r, 0.75f, 0.3f, 1.f));
		m_text_manager->render_text(text_shader, "ESC- Quit", (float)engine::application::window().width() / 1.8, (float)engine::application::window().height() / 2, 0.5f, glm::vec4(0.5f, 0.75f, r, 1.f));
		m_text_manager->render_text(text_shader, "Enter- Start", (float)engine::application::window().width() / 1.5, (float)engine::application::window().height() / 2, 0.5f, glm::vec4(0.25f, r, r, 1.f));
		m_text_manager->render_text(text_shader, "SPC- Shoot", (float)engine::application::window().width() / 1.25, (float)engine::application::window().height() / 2, 0.5f, glm::vec4(1.f, r, 0.f, 1.f));
		m_text_manager->render_text(text_shader, "E before Enter- easy", (float)engine::application::window().width() / 1.25, (float)engine::application::window().height() / 4, 0.5f, glm::vec4(1.f, r, r, 1.f));
		m_text_manager->render_text(text_shader, "H before Enter- hard", (float)engine::application::window().width() / 1.25, (float)engine::application::window().height() / 3, 0.5f, glm::vec4(1.f, r, r, 1.f));

		//display the current mode on introscreen
		if (easy) {
			m_text_manager->render_text(text_shader, "Current Mode - easy", (float)engine::application::window().width() / 1.25, (float)engine::application::window().height() / 5, 0.5f, glm::vec4(r, 1.f, r, 1.f));
		}
		else {
			m_text_manager->render_text(text_shader, "Current Mode - hard", (float)engine::application::window().width() / 1.25, (float)engine::application::window().height() / 5, 0.5f, glm::vec4(1.f, r, r, 1.f));
		}

		//title
		m_text_manager->render_text(text_shader, "Psychedelic Invaders", (float)engine::application::window().width() /2 , (float)engine::application::window().height() -200.f, 1.f, glm::vec4(0.75f, r, 0.3f, 1.f));

	}

	//HUD
	if (!m_startscreen) {

		const auto text_shader = engine::renderer::shaders_library()->get("text_2D");

		//converts variables to strings to render as text
		std::string timer = std::to_string(seconds);
		std::string current_score = std::to_string(score);
		std::string lives_left = std::to_string(lives);

		//score render
		m_text_manager->render_text(text_shader, "Score: " + current_score, 10.f, (float)engine::application::window().height() - 25.f, 0.5f, glm::vec4(r, 0.8f, r-0.2f, 1.f));
		//lives remaining render
		m_text_manager->render_text(text_shader, "Lives: " + lives_left, (float)engine::application::window().width() / 3, (float)engine::application::window().height() -25.f, 0.5f, glm::vec4(r, 0.8f, r - 0.2f, 1.f));
		//time survived render
		m_text_manager->render_text(text_shader, "Timer: " + timer,  (float)engine::application::window().width() / 1.3, (float)engine::application::window().height() - 25.f, 0.5f, glm::vec4(r, 0.3f, r -0.2, 1.f));
		//crosshair render
		m_text_manager->render_text (text_shader, "[x]", (float)engine::application::window().width() / 2, (float)engine::application::window().height() / 2, 1.f, glm::vec4(1.f , r , r * 0.4f, 1.f));

		
	}

}
void example_layer::on_event(engine::event& event) 
{ 
    if(event.event_type() == engine::event_type_e::key_pressed) 
    { 
        auto& e = dynamic_cast<engine::key_pressed_event&>(event); 
        if(e.key_code() == engine::key_codes::KEY_TAB) 
        { 
            engine::render_command::toggle_wireframe();
        }

		//start game
		if (e.key_code() == engine::key_codes::KEY_ENTER) {
			m_startscreen = false;

			//stop intro music
			m_audio_manager->stop_all();

			// playing in game music once game starts
			m_audio_manager->play("music");
		}

		//shooting ballistic
		if (!m_startscreen) {

			if (e.key_code() == engine::key_codes::KEY_SPACE){
			m_ballistic.fire(m_3d_camera, 1.0f);
			// playing shooting sound
			m_audio_manager->play("bullet");

			
			}

		}
		// easy and hard modes
		if (m_startscreen) {
			if (e.key_code() == engine::key_codes::KEY_E) {
				lives = 3;
				easy = true;
				

			}
			if (e.key_code() == engine::key_codes::KEY_H) {
				lives = 1;
				easy = false;
				

			}
		}
		
    }
}


/*void example_layer::check_bounce()
{
	//if (m_prev_sphere_y_vel < 0.f && m_game_objects.at(1)->velocity().y > 0.f)
		//m_audio_manager->play("bounce");
	m_prev_sphere_y_vel = m_game_objects.at(1)->velocity().y;
}*/



