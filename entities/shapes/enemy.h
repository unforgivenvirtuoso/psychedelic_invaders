#pragma once
#include <engine.h>
namespace engine
{
	class mesh;
	
	class enemy 
	{
	public:
		enemy(const engine::game_object_properties props);
		
		/// \brief Constructor
		enemy(std::vector<glm::vec3> vertices);
		
		/// \brief Destructor
		~enemy();
		/// \brief Getter methods
		std::vector<glm::vec3> vertices() const { return m_vertices; }
		ref<engine::mesh> mesh() const { return m_mesh; }
		static ref<enemy> create(std::vector<glm::vec3> vertices);

		

	
	private:
		/// \brief Fields
		// vertices of the tetrahedron specified by a vector of positions.
		std::vector<glm::vec3> m_vertices;
		ref<engine::mesh> m_mesh;

		
	};
}
