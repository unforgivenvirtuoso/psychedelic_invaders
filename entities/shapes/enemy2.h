#pragma once
#include <engine.h>

namespace engine
{
	class mesh;

	class enemy2
	{
	public:
		enemy2(const engine::game_object_properties props);


		enemy2(std::vector<glm::vec3> vertices);

	
		~enemy2();
	
		std::vector<glm::vec3> vertices() const { return m_vertices; }
		ref<engine::mesh> mesh() const { return m_mesh; }
		static ref<enemy2> create(std::vector<glm::vec3> vertices);




	private:
	
		std::vector<glm::vec3> m_vertices;
		ref<engine::mesh> m_mesh;


	};
}
