#include "example_layer.h"
#include "platform/opengl/gl_shader.h"

#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp>
#include "engine/events/key_event.h"
#include "engine/utils/track.h"

bool intro_skipped = false;

example_layer::example_layer() 
    :m_2d_camera(-1.6f, 1.6f, -0.9f, 0.9f), 
    m_3d_camera((float)engine::application::window().width(), (float)engine::application::window().height())


{

	m_fog = true;
	glm::vec3 m_fog_colour = glm::vec3(0.5f);
	float m_fog_factor_type = 0.1f;

    // Hide the mouse and lock it inside the window
    //engine::input::anchor_mouse(true);
    engine::application::window().hide_mouse_cursor();

	// Initialise audio and play background music
	m_audio_manager = engine::audio_manager::instance();
	m_audio_manager->init();
	m_audio_manager->load_sound("assets/audio/Jump-SoundBible.com-1007297584.mp3", engine::sound_type::event, "jump"); // Royalty free sound from soundbible.com
	m_audio_manager->load_sound("assets/audio/JeffSpeed68_-_Plastic_Motions.mp3", engine::sound_type::track, "music");  // Royalty free music from http://dig.ccmixter.org/games
	m_audio_manager->load_sound("assets/audio/hard_shoes_1-daniel_simon.mp3", engine::sound_type::event, "walk"); // Royalty free sound from soundbible.com
	m_audio_manager->loop("music", true);
	m_audio_manager->play("music");
	m_audio_manager->volume("music", 0.5f);
	//setup walking sound to play when unpaused
	m_audio_manager->loop("walk", true);
	m_audio_manager->play("walk");
	m_audio_manager->pause("walk");
	m_audio_manager->volume("walk", 0.4f);

	m_audio_manager->volume("jump", 0.4f);
	
	


	// Initialise the shaders, materials and lights
	auto mesh__material_shader = engine::renderer::shaders_library()->get("mesh_material");
	auto mesh_lighting_shader = engine::renderer::shaders_library()->get("mesh_lighting");
	auto text_shader = engine::renderer::shaders_library()->get("text_2D");
	auto animated_mesh_shader = engine::renderer::shaders_library()->get("animated_mesh");

	m_directionalLight.Color = glm::vec3(1.0f, 1.0f, 1.0f);
	m_directionalLight.AmbientIntensity = 0.01f;
	m_directionalLight.DiffuseIntensity = 0.90f;
	m_directionalLight.Direction = glm::normalize(glm::vec3(-1.0f, -1.0f, 0.0f));

	m_pointLight.Color = glm::vec3(1.0f, 1.0f, 1.0f);
	m_pointLight.AmbientIntensity = 0.32f;
	m_pointLight.DiffuseIntensity = 0.55f;
	m_pointLight.Position = glm::normalize(glm::vec3(0.0f, 2.0f, 0.0f));

	m_spotLight.Color = glm::vec3(1.0f, 1.0f, 1.0f);
	m_spotLight.AmbientIntensity = 0.32f;
	m_spotLight.DiffuseIntensity = 0.55f;
	m_spotLight.Position = glm::normalize(glm::vec3(1.0f, 2.0f, 0.0f));
	m_spotLight.Direction = glm::normalize(glm::vec3(1.0f, -1.0f, 0.0f));
	

	// set color texture unit
	std::dynamic_pointer_cast<engine::gl_shader>(animated_mesh_shader)->bind();
	std::dynamic_pointer_cast<engine::gl_shader>(animated_mesh_shader)->set_uniform("gColorMap", 0);
	std::dynamic_pointer_cast<engine::gl_shader>(animated_mesh_shader)->set_uniform("lighting_on", true);
	m_directionalLight.submit(animated_mesh_shader);
	std::dynamic_pointer_cast<engine::gl_shader>(animated_mesh_shader)->set_uniform("gMatSpecularIntensity", 0.5f);
	std::dynamic_pointer_cast<engine::gl_shader>(animated_mesh_shader)->set_uniform("gSpecularPower", 5.f);
	std::dynamic_pointer_cast<engine::gl_shader>(animated_mesh_shader)->set_uniform("transparency", 1.0f);
	std::dynamic_pointer_cast<engine::gl_shader>(animated_mesh_shader)->set_uniform("fog_colour", m_fog_colour);
	std::dynamic_pointer_cast<engine::gl_shader>(animated_mesh_shader)->set_uniform("fog_factor_type", m_fog_factor_type);
	std::dynamic_pointer_cast<engine::gl_shader>(mesh__material_shader)->bind();
	std::dynamic_pointer_cast<engine::gl_shader>(mesh__material_shader)->set_uniform("lighting_on", true);
	//m_directionalLight.submit(mesh__material_shader);
	std::dynamic_pointer_cast<engine::gl_shader>(mesh__material_shader)->set_uniform("gMatSpecularIntensity", 1.f);
	std::dynamic_pointer_cast<engine::gl_shader>(mesh__material_shader)->set_uniform("gSpecularPower", 10.f);
	std::dynamic_pointer_cast<engine::gl_shader>(animated_mesh_shader)->set_uniform("fog_colour", m_fog_colour);
	std::dynamic_pointer_cast<engine::gl_shader>(animated_mesh_shader)->set_uniform("fog_factor_type", m_fog_factor_type);
	std::dynamic_pointer_cast<engine::gl_shader>(mesh_lighting_shader)->bind();
	std::dynamic_pointer_cast<engine::gl_shader>(mesh_lighting_shader)->set_uniform("gColorMap", 0);
	std::dynamic_pointer_cast<engine::gl_shader>(mesh_lighting_shader)->set_uniform("lighting_on", true);
	m_directionalLight.submit(mesh_lighting_shader);
	std::dynamic_pointer_cast<engine::gl_shader>(mesh_lighting_shader)->set_uniform("gMatSpecularIntensity", 0.5f);
	std::dynamic_pointer_cast<engine::gl_shader>(mesh_lighting_shader)->set_uniform("gSpecularPower", 5.f);
	std::dynamic_pointer_cast<engine::gl_shader>(mesh_lighting_shader)->set_uniform("transparency", 1.0f);
	std::dynamic_pointer_cast<engine::gl_shader>(animated_mesh_shader)->set_uniform("fog_colour", m_fog_colour);
	std::dynamic_pointer_cast<engine::gl_shader>(animated_mesh_shader)->set_uniform("fog_factor_type", m_fog_factor_type);
	std::dynamic_pointer_cast<engine::gl_shader>(text_shader)->bind();
	std::dynamic_pointer_cast<engine::gl_shader>(text_shader)->set_uniform("projection",
		glm::ortho(0.f, (float)engine::application::window().width(), 0.f,
		(float)engine::application::window().height()));

		m_material = engine::material::create(1.0f, glm::vec3(1.0f, 0.1f, 0.07f),
		glm::vec3(1.0f, 0.1f, 0.07f), glm::vec3(0.5f, 0.5f, 0.5f), 1.0f);


	// Skybox texture from http://www.custommapmakers.org/skyboxes.php
	m_skybox = engine::skybox::create(50.f,
		{ engine::texture_2d::create("assets/textures/skybox/SkyboxFront.bmp", true),
		  engine::texture_2d::create("assets/textures/skybox/SkyboxRight.bmp", true),
		  engine::texture_2d::create("assets/textures/skybox/SkyboxBack.bmp", true),
		  engine::texture_2d::create("assets/textures/skybox/SkyboxLeft.bmp", true),
		  engine::texture_2d::create("assets/textures/skybox/SkyboxTop.bmp", true),
		  engine::texture_2d::create("assets/textures/skybox/SkyboxBottom.bmp", true)
		});

//	//Creates wolf mesh
//	engine::ref<engine::model> m_wolf_mesh = engine::model::create("assets/models/animated/wolf/Wolf_dae.dae");
///*	m_wolf_mesh->LoadAnimationFile("assets/models/animated/wolf/Wolf_One_dae.dae");
//	m_wolf_mesh->switch_root_movement(false);*/
//
//	engine::game_object_properties wolf_props;
//	//wolf_props.animated_mesh = m_wolf_mesh;
//	wolf_props.meshes = m_wolf_mesh->meshes();
//	wolf_props.textures = m_wolf_mesh->textures();
//	wolf_props.scale = glm::vec3(1.f / glm::max(m_wolf_mesh->size().x, glm::max(m_wolf_mesh->size().y, m_wolf_mesh->size().z)));
//	wolf_props.position = glm::vec3(3.0f, 0.5f, -5.0f);
//	//wolf_props.type = 0;
//	wolf_props.bounding_shape = m_wolf_mesh->size() / 2.f * wolf_props.scale.x;
//	m_wolf = engine::game_object::create(wolf_props);
//	m_enemy.initialise(m_wolf);


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

	m_player.initialise(m_mannequin);


	// Load the terrain texture and create a terrain mesh. Create a terrain object. Set its properties
	//Terrain texture from https://www.creativeswall.com/30-free-sand-textures/


	std::vector<engine::ref<engine::texture_2d>> terrain_textures = { engine::texture_2d::create("assets/textures/terrain.bmp", false) };
	engine::ref<engine::terrain> terrain_shape = engine::terrain::create(100.f, 0.5f, 100.f);
	engine::game_object_properties terrain_props;
	terrain_props.meshes = { terrain_shape->mesh() };
	terrain_props.textures = terrain_textures;
	terrain_props.is_static = true;
	terrain_props.type = 0 ;
	terrain_props.bounding_shape = glm::vec3(10.f, 0.5f, 10.f);
	terrain_props.restitution = 0.92f;
	m_terrain = engine::game_object::create(terrain_props);

	// Load the cow model. Create a cow object. Set its properties
	engine::ref <engine::model> cow_model = engine::model::create("assets/models/static/cow4.3ds");
	engine::game_object_properties cow_props;
	cow_props.meshes = cow_model->meshes();
	cow_props.textures = cow_model->textures();
	float cow_scale = 1.f / glm::max(cow_model->size().x, glm::max(cow_model->size().y, cow_model->size().z));
	cow_props.position = { -4.f,0.5f, -5.f };
	cow_props.scale = glm::vec3(cow_scale);
	cow_props.bounding_shape = cow_model->size() / 2.f * cow_scale;
	m_cow = engine::game_object::create(cow_props);
	m_enemy.initialise(m_cow);

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

	// Load truck model. Create a truck object. Set its properties
	//texture and model from https://www.turbosquid.com/3d-models/weathered-truck-ruins-obj-free/668299
	engine::ref <engine::model> truck_model = engine::model::create("assets/models/static/truck/truck.obj");
	engine::game_object_properties truck_props;
	truck_props.meshes = truck_model->meshes();
	truck_props.textures = truck_model->textures();
	float truck_scale = 3.f / glm::max(truck_model->size().x, glm::max(truck_model->size().y, truck_model->size().z));
	truck_props.position = { 5.f,0.6f, -6.f };
	truck_props.scale = glm::vec3(truck_scale);
	truck_props.bounding_shape = truck_model->size() / 2.f * truck_scale;
	m_truck = engine::game_object::create(truck_props);

	// Load pile of logs model. Create an object. Set its properties
//texture and model from https://free3d.com/3d-model/roundwood-41562.html
	engine::ref <engine::model> logs_model = engine::model::create("assets/models/static/wood/Wood.obj");
	engine::game_object_properties logs_props;
	logs_props.meshes = logs_model->meshes();
	logs_props.textures = logs_model->textures();
	float logs_scale = 3.f / glm::max(logs_model->size().x, glm::max(logs_model->size().y, logs_model->size().z));
	logs_props.position = { 4.f,0.6f, -3.f };
	logs_props.scale = glm::vec3(logs_scale);
	logs_props.bounding_shape = logs_model->size() / 2.f * logs_scale;
	m_logs = engine::game_object::create(logs_props);

	// Load well model. Create an object. Set its properties
//texture and model from https://free3d.com/3d-model/stone-well-34248.html
	engine::ref <engine::model> well_model = engine::model::create("assets/models/static/well/well.obj");
	engine::game_object_properties well_props;
	well_props.meshes = well_model->meshes();
	well_props.textures = well_model->textures();
	float well_scale = 2.f / glm::max(well_model->size().x, glm::max(well_model->size().y, well_model->size().z));
	well_props.position = { -4.f,0.5f, 5.f };
	well_props.scale = glm::vec3(well_scale);
	well_props.bounding_shape = well_model->size() / 2.f * well_scale;
	m_well = engine::game_object::create(well_props);



	//Create rock from primitives. Set its properties
	std::vector<glm::vec3> rock_vertices;
	//TOP PART
	rock_vertices.push_back(glm::vec3(0.f, 5.f, 0.f));		//0
	rock_vertices.push_back(glm::vec3(0.f, 0.f, 5.f));		//1
	rock_vertices.push_back(glm::vec3(-3.f, 0.f, -2.f));	//2
	//BOTTOM PART

	rock_vertices.push_back(glm::vec3(5.f, 0.f, -5.f));	//3
	rock_vertices.push_back(glm::vec3(0.f, -1.f, 0.f));	//4
	rock_vertices.push_back(glm::vec3(0.f, 0.f, 2.f));	//5
	rock_vertices.push_back(glm::vec3(1.f, 0.f, 5.f));

	//	//Texture from  https://www.turbosquid.com/FullPreview/Index.cfm/ID/371843
	std::vector<engine::ref<engine::texture_2d>> rock_textures =
	{ engine::texture_2d::create("assets/textures/rock.bmp", false) };

	engine::ref<engine::rock> rock_shape = engine::rock::create(rock_vertices);
	engine::game_object_properties rock_props;
	rock_props.position = { 4.f, 1.f, 0.f };
	rock_props.meshes = { rock_shape->mesh() };
	rock_props.textures = rock_textures;
	rock_props.scale = glm::vec3(0.5f,0.5f,0.5f);
	m_rock = engine::game_object::create(rock_props);


	//Create spaceship from primitives. Set its properties
	std::vector<glm::vec3> spaceship_vertices;
	//TOP PART
	spaceship_vertices.push_back(glm::vec3(0.f, 5.f, 0.f));		//0
	spaceship_vertices.push_back(glm::vec3(0.f,0.f, 10.f));		//1
	spaceship_vertices.push_back(glm::vec3(-5.f, 0.f, -5.f));	//2
	//BOTTOM PART

	spaceship_vertices.push_back(glm::vec3(5.f, 0.f, -5.f));	//3
	spaceship_vertices.push_back(glm::vec3(0.f, -1.f, 0.f));	//4
	spaceship_vertices.push_back(glm::vec3(0.f, 0.f, 2.f));		//5


	//Texture from  https://www.texturepalace.com/download-rusty-metal-texture/
	std::vector<engine::ref<engine::texture_2d>> spaceship_textures =
	{ engine::texture_2d::create("assets/textures/rustymetal.jpg", false) };


	engine::ref<engine::spaceship> spaceship_shape = engine::spaceship::create(spaceship_vertices);
	engine::game_object_properties spaceship_props;
	spaceship_props.position = { 0.f, 5.f, -20.f };
	spaceship_props.meshes = { spaceship_shape->mesh() };
	spaceship_props.textures = spaceship_textures;
	m_spaceship = engine::game_object::create(spaceship_props);


	m_game_objects.push_back(m_terrain);

	m_text_manager = engine::text_manager::create();

	m_skinned_mesh->switch_animation(2);

	m_cross_fade = cross_fade::create("assets/textures/Intro/intro.png", 5.0f, 1.6f, 0.9f);

	m_billboard = billboard::create("assets/textures/Fume_camp_sample.max", 4, 5, 16);
}

example_layer::~example_layer() {}

void example_layer::on_update(const engine::timestep& time_step) 
{
	if(m_pointLight.Position.x<50)
		m_pointLight.Position.x += 0.02f; 

	if (m_pointLight.Position.x > 50)
		m_pointLight.Position.x = -20.f;
	
	m_player.update_camera(m_3d_camera);


	//Power-up trigger -- increase speed while in range

	if (glm::distance(m_player.player_position(), m_well->position()) < 5.f) {
		std::cout << "TRIGGER PLAYER BOOST" << std::endl;
		m_player.m_speed = 2.f;
	}
	std::cout << m_cow->position() << std::endl;



	//m_3d_camera.on_update(time_step);
	

	//m_physics_manager->dynamics_world_update(m_game_objects, double(time_step));

	m_billboard->on_update(time_step);

	m_player.on_update(time_step);
	m_enemy.on_update(time_step);

	//intro trigger
	m_cross_fade->on_update(time_step);


	//sound when walking
	walking_sound();


	if (intro_skipped == false) {
		m_cross_fade->activate();
	}

} 

void example_layer::on_render() 
{
    engine::render_command::clear_color({0.2f, 0.3f, 0.3f, 1.0f}); 
    engine::render_command::clear();

    //const auto textured_shader = engine::renderer::shaders_library()->get("mesh_static");
    //engine::renderer::begin_scene(m_3d_camera, textured_shader);

	const auto textured_lighting_shader = engine::renderer::shaders_library()->get("mesh_lighting");
	engine::renderer::begin_scene(m_3d_camera, textured_lighting_shader);

	// Set up some of the scene's parameters in the shader
	std::dynamic_pointer_cast<engine::gl_shader>(textured_lighting_shader)->set_uniform("gEyeWorldPos", m_3d_camera.position());
	std::dynamic_pointer_cast<engine::gl_shader>(textured_lighting_shader)->set_uniform("fog_on", m_fog);
	// Position the skybox centred on the player and render it
	glm::mat4 skybox_tranform(1.0f);
	skybox_tranform = glm::translate(skybox_tranform, m_3d_camera.position());
	for (const auto& texture : m_skybox->textures())
	{
		texture->bind();
	}
	engine::renderer::submit(textured_lighting_shader, m_skybox, skybox_tranform);

	engine::renderer::submit(textured_lighting_shader, m_terrain);

	std::dynamic_pointer_cast<engine::gl_shader>(textured_lighting_shader)->
		set_uniform("gNumPointLights", (int)num_point_lights);
	m_pointLight.submit(textured_lighting_shader, 0);
	std::dynamic_pointer_cast<engine::gl_shader>(textured_lighting_shader)->
		set_uniform("gNumPointLights", (int)num_spot_lights);
	m_spotLight.submit(textured_lighting_shader, 0);

	glm::mat4 tree_transform(1.0f);
	tree_transform = glm::translate(tree_transform, m_tree->position());
	tree_transform = glm::rotate(tree_transform, m_tree->rotation_amount(), m_tree->rotation_axis());
	tree_transform = glm::scale(tree_transform, m_tree->scale());
	engine::renderer::submit(textured_lighting_shader, tree_transform, m_tree);
	
	glm::mat4 cow_transform(1.0f);
	cow_transform = glm::translate(cow_transform, m_cow->position());
	cow_transform = glm::rotate(cow_transform, m_cow->rotation_amount(), m_cow->rotation_axis());
	cow_transform = glm::scale(cow_transform, m_cow->scale());
	engine::renderer::submit(textured_lighting_shader, cow_transform, m_cow);

	glm::mat4 truck_transform(1.0f);
	truck_transform = glm::translate(truck_transform, m_truck->position());
	truck_transform = glm::rotate(truck_transform, -1.8f, m_truck->rotation_axis());
	truck_transform = glm::scale(truck_transform, m_truck->scale());
	engine::renderer::submit(textured_lighting_shader, truck_transform, m_truck);
	truck_transform = glm::translate(truck_transform, glm::vec3(-10.f, 0.f, 15.0f));
	truck_transform = glm::rotate(truck_transform, -0.78f, m_truck->rotation_axis());
	engine::renderer::submit(textured_lighting_shader, truck_transform, m_truck);

	glm::mat4 logs_transform(1.0f);
	logs_transform = glm::translate(logs_transform, m_logs->position());
	logs_transform = glm::rotate(logs_transform, -0.5f, m_logs->rotation_axis());
	logs_transform = glm::scale(logs_transform, m_logs->scale());
	engine::renderer::submit(textured_lighting_shader, logs_transform, m_logs);
	logs_transform = glm::translate(logs_transform, glm::vec3(-10.f, 0.f, 15.0f));
	logs_transform = glm::rotate(logs_transform, -0.78f, m_logs->rotation_axis());
	engine::renderer::submit(textured_lighting_shader, logs_transform, m_logs);

	glm::mat4 well_transform(1.0f);
	well_transform = glm::translate(well_transform, m_well->position());
	well_transform = glm::rotate(well_transform, -1.8f, m_well->rotation_axis());
	well_transform = glm::scale(well_transform, m_well->scale());
	engine::renderer::submit(textured_lighting_shader, well_transform, m_well);



	glm::mat4 spaceship_transform(1.0f);

	for (int i = 1; i < 5; i++) {
		spaceship_transform = glm::translate(spaceship_transform, glm::vec3(15.f*i, 5.0f*i, 5.0f*i));
		spaceship_transform = glm::rotate(spaceship_transform, -1.57f, m_spaceship->rotation_axis());
		spaceship_transform = glm::scale(spaceship_transform, m_spaceship->scale());
		engine::renderer::submit(textured_lighting_shader, spaceship_transform, m_spaceship);
	}

	glm::mat4 rock_transform(1.0f);

		rock_transform = glm::translate(rock_transform, m_rock->position());
		rock_transform = glm::rotate(rock_transform, -1.57f, m_rock->rotation_axis());
		rock_transform = glm::scale(rock_transform, m_rock->scale());
		engine::renderer::submit(textured_lighting_shader, rock_transform, m_rock);

    engine::renderer::end_scene();

	// Set up material shader. (does not render textures, renders materials instead)
	const auto material_shader = engine::renderer::shaders_library()->get("mesh_material");
	engine::renderer::begin_scene(m_3d_camera, material_shader);

	m_material->submit(material_shader);
	std::dynamic_pointer_cast<engine::gl_shader>(material_shader)->set_uniform("gEyeWorldPos", m_3d_camera.position());


	engine::renderer::end_scene();

	const auto animated_mesh_shader = engine::renderer::shaders_library()->get("animated_mesh");
	engine::renderer::begin_scene(m_3d_camera, animated_mesh_shader);
	std::dynamic_pointer_cast<engine::gl_shader>(animated_mesh_shader)->set_uniform("gEyeWorldPos", m_3d_camera.position());

	glm::mat4 aniTransform = glm::mat4(1.0f);

	engine::renderer::submit(animated_mesh_shader, m_player.object());

	engine::renderer::end_scene();

	// Render intro text
	engine::renderer::begin_scene(m_2d_camera, textured_lighting_shader);
	m_cross_fade->on_render(textured_lighting_shader);
	engine::renderer::end_scene();

	//Render HUD
	const auto text_shader = engine::renderer::shaders_library()->get("text_2D");
	char txt[128];
	
	memset(txt, 0, 128);
	sprintf_s(txt, "Lives: %i", m_lives);
	m_text_manager->render_text(text_shader, txt, 10.f, 25.f, 0.5f, glm::vec4(0.8f, 0.8f, 0.8f, 1.f));
	memset(txt, 0, 128);
	sprintf_s(txt, "Score: %i", m_score);
	m_text_manager->render_text(text_shader, txt, 10.f, 50.f, 0.5f, glm::vec4(0.8f, 0.8f, 0.8f, 1.f));

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
		if (engine::input::key_pressed(engine::key_codes::KEY_ENTER))
		{
			intro_skipped = true;

		}
		if (e.key_code() == engine::key_codes::KEY_5)
		{
			m_fog = !m_fog;
		}

		if (e.key_code() == engine::key_codes::KEY_H && m_lives>0)
		{
			m_lives--;
		}
		if (e.key_code() == engine::key_codes::KEY_P)
		{
			m_score++;
		}
		if (e.key_code() == engine::key_codes::KEY_2)
		{
			m_billboard->activate(glm::vec3(0.f, 5.f, -10.f), 4.f, 4.f);
		}

		if (glm::distance(m_player.player_position(), m_cow->position()) < 2.f &&e.key_code() == engine::key_codes::KEY_E) {
			m_enemy.set_state_flee();
			m_score++;
		}
		else if (glm::distance(m_player.player_position(), m_cow->position()) >10.f) {
			m_enemy.set_state_idle();
		}
		//sound when jumping
		jumping_sound();

    }



}

int example_layer::get_lives()
{
	return m_lives;
}

void example_layer::jumping_sound()
{
	bool jumpSound = false;
	if (engine::input::key_pressed(engine::key_codes::KEY_SPACE)) {
		jumpSound = true;
	}
	if (jumpSound == true) { m_audio_manager->play("jump"); }
}
void example_layer::walking_sound()
{
	// lacks proper input check that doesnt loop
	bool walkSound = false;
	if (engine::input::key_pressed(engine::key_codes::KEY_W) ||
		engine::input::key_pressed(engine::key_codes::KEY_A) ||
		engine::input::key_pressed(engine::key_codes::KEY_S) ||
		engine::input::key_pressed(engine::key_codes::KEY_D)) {
		walkSound = true;
		m_audio_manager->unpause("walk");
	}
	else { m_audio_manager->pause("walk"); }

}

