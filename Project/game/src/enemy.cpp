#include "enemy.h"
#include "pch.h"
#include "engine/core/input.h"
#include "engine/key_codes.h"

enemy::enemy() : m_timer(0.0f), m_speed(0.5f), FSM_STATE('I')
{
}

enemy::~enemy()
{}

void enemy::initialise(engine::ref<engine::game_object> object)
{
	m_object = object;
	m_object->set_forward(glm::vec3(0.f, 0.f, -1.f));
	m_object->set_position(glm::vec3(-4.f, 0.5f, -5.f));
	
}

void enemy::on_update(const engine::timestep& time_step)
{

	

	m_object->set_position(m_object->position() += m_object->forward() * m_speed * (float)time_step);
	m_object->set_rotation_amount(atan2(m_object->forward().x, m_object->forward().z));

	switch (FSM_STATE) {
	//Idle state
	case 'I':
		m_speed = 0.5f;
		turn(0.2f * time_step);
		break;
	//Agggressive state
	case 'A':


		break;
	//Flee state
	case 'F':
		m_speed = 5.f;
		break;

	}

	//if (engine::input::key_pressed(engine::key_codes::KEY_A))			//left
	//	turn(1.0f * time_step);
	//else if (engine::input::key_pressed(engine::key_codes::KEY_D))		//right
	//	turn(-1.0f * time_step);
	//else if (engine::input::key_pressed(engine::key_codes::KEY_W)) {	//front
	//	
	//	if (engine::input::key_pressed(engine::key_codes::KEY_LEFT_SHIFT))
	//	{
	//		m_speed = 2.5f;
	//	}
	//	m_object->set_position(m_object->position() += m_object->forward() * m_speed * (float)time_step);
	//	m_speed = 1.f;
	//}
	//else if (engine::input::key_pressed(engine::key_codes::KEY_S)) {	 //back 
	//	m_object->set_position(m_object->position() -= m_object->forward() * m_speed * (float)time_step);
	//}

	
	
}

void enemy::turn(float angle)
{
	m_object->set_forward(glm::rotate(m_object->forward(), angle, glm::vec3(0.f, 1.f, 0.f)));
}

void enemy::set_state_flee()
{
	FSM_STATE = 'F';
	std::cout<<"TRIGGER FLEE" << std::endl;
}

void enemy::set_state_idle()
{
	FSM_STATE = 'I';
	std::cout << "TRIGGER IDLE" << std::endl;
}
