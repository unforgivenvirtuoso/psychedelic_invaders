#include "pch.h"
#include "enemy.h"
#include <engine.h>



engine::enemy::enemy(const engine::game_object_properties props) {

}



engine::enemy::enemy(std::vector<glm::vec3> vertices) : m_vertices(vertices){


	std::vector<glm::vec3> normals;
	// front normal
	normals.push_back(glm::cross(vertices.at(0) - vertices.at(2), vertices.at(0) -
		vertices.at(1)));
	// left normal
	normals.push_back(glm::cross(vertices.at(0) - vertices.at(3), vertices.at(0) -
		vertices.at(2)));
	// right normal
	normals.push_back(glm::cross(vertices.at(0) - vertices.at(1), vertices.at(0) -
		vertices.at(3)));
	// bottom normal
	normals.push_back(glm::cross(vertices.at(1) - vertices.at(2), vertices.at(1) -
		vertices.at(3)));



	

		std::vector<mesh::vertex> enemy_vertices
	{
		//front

		// position normal tex coord
		 { vertices.at(0), normals.at(0), { 0.f, 0.f } },
		 { vertices.at(2), normals.at(0), { 1.f, 0.f } },  
		 { vertices.at(1), normals.at(0), { 0.5f, 1.f } }, 

		 //left
		  { vertices.at(0), normals.at(1), { 0.f, 0.f } },
		  { vertices.at(3), normals.at(1), { 1.f, 0.f } }, 
		 { vertices.at(2), normals.at(1), { 0.5f, 1.f } }, 

		 //right
		 { vertices.at(0), normals.at(2), { 0.f, 0.f } },
		 { vertices.at(1), normals.at(2), { 1.f, 0.f } }, 
		 { vertices.at(3), normals.at(2), { 0.5f, 1.f } }, 

		 //back
		 { vertices.at(1), normals.at(3), { 0.f, 0.f } },
		 { vertices.at(2), normals.at(3), { 1.f, 0.f } }, 
		 { vertices.at(3), normals.at(3), { 0.5f, 1.f } }, 

		
		 
	};


		const std::vector<uint32_t> enemy_indices
		{
		 0, 1, 2, //front
		 3, 4, 5, //left
		 6, 7, 8, //right
		 9, 10, 11 //back
		 
		};

		


		m_mesh = engine::mesh::create(enemy_vertices, enemy_indices);


}



engine::enemy::~enemy() {}
engine::ref<engine::enemy> engine::enemy::create(std::vector<glm::vec3>
	vertices)
{
	return std::make_shared<engine::enemy>(vertices);
}
