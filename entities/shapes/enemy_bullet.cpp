#include "pch.h"
#include "enemy_bullet.h"
#include <engine.h>



engine::enemy_bullet::enemy_bullet(const engine::game_object_properties props) {

}



engine::enemy_bullet::enemy_bullet(std::vector<glm::vec3> vertices) : m_vertices(vertices) {


	std::vector<glm::vec3> normals;

	normals.push_back(glm::cross(vertices.at(0) - vertices.at(2), vertices.at(0) -
		vertices.at(1)));

	normals.push_back(glm::cross(vertices.at(0) - vertices.at(3), vertices.at(0) -
		vertices.at(2)));
	
	normals.push_back(glm::cross(vertices.at(0) - vertices.at(1), vertices.at(0) -
		vertices.at(3)));
	
	normals.push_back(glm::cross(vertices.at(1) - vertices.at(2), vertices.at(1) -
		vertices.at(3)));





	std::vector<mesh::vertex> enemy_bullet_vertices
	{
		
		 { vertices.at(0), normals.at(0), { 0.f, 0.f } },
		 { vertices.at(2), normals.at(0), { 1.f, 0.f } },
		 { vertices.at(1), normals.at(0), { 0.5f, 1.f } },

		
		  { vertices.at(0), normals.at(1), { 0.f, 0.f } },
		  { vertices.at(3), normals.at(1), { 1.f, 0.f } },
		 { vertices.at(2), normals.at(1), { 0.5f, 1.f } },

		 
		 { vertices.at(2), normals.at(2), { 0.f, 0.f } },
		 { vertices.at(1), normals.at(2), { 1.f, 0.f } },
		 { vertices.at(3), normals.at(2), { 0.5f, 1.f } },

		 
		 { vertices.at(1), normals.at(3), { 0.f, 0.f } },
		 { vertices.at(2), normals.at(3), { 1.f, 0.f } },
		 { vertices.at(3), normals.at(3), { 0.5f, 1.f } },

		 { vertices.at(3), normals.at(0), { 0.f, 0.f } },
		 { vertices.at(2), normals.at(0), { 1.f, 0.f } },
		 { vertices.at(1), normals.at(0), { 0.5f, 1.f } },

		 
		  { vertices.at(1), normals.at(1), { 0.f, 0.f } },
		  { vertices.at(3), normals.at(1), { 1.f, 0.f } },
		 { vertices.at(2), normals.at(1), { 0.5f, 1.f } },

		 
		 { vertices.at(2), normals.at(2), { 0.f, 0.f } },
		 { vertices.at(1), normals.at(2), { 1.f, 0.f } },
		 { vertices.at(3), normals.at(2), { 0.5f, 1.f } },

		 
		



	};


	const std::vector<uint32_t> enemy_bullet_indices
	{
	 0, 1, 2, 
	 3, 4, 5, 
	 6, 7, 8, 
	 9, 10, 11,
	 12, 13, 14,
	 15, 16, 17,
	 18, 19, 20

	};




	m_mesh = engine::mesh::create(enemy_bullet_vertices, enemy_bullet_indices);


}



engine::enemy_bullet::~enemy_bullet() {}
engine::ref<engine::enemy_bullet> engine::enemy_bullet::create(std::vector<glm::vec3>
	vertices)
{
	return std::make_shared<engine::enemy_bullet>(vertices);
}
