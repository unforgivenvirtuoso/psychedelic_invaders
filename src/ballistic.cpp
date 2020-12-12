#include "pch.h"
#include "Ballistic.h"

ballistic::ballistic()
{}

ballistic::~ballistic()
{}

void ballistic::initialise(engine::ref<engine::game_object> object)
{
	m_object = object;
}

void ballistic::fire(const engine::perspective_camera& camera, float speed)
{
	m_object->set_position(camera.position());
	m_object->set_forward(camera.front_vector());
	
	m_speed =  20 * speed;
}

void ballistic::on_update(const engine::timestep& time_step)
{
	m_object->set_position(m_object->position() + m_object->forward() * (float)time_step * m_speed);

}

void ballistic::on_render(const engine::ref<engine::shader>& shader)
{
	engine::renderer::submit(shader, m_object);
}
