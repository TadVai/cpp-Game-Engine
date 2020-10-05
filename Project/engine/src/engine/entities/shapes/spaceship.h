#pragma once

namespace engine
{
	class mesh;
	/// \brief Class creating a spaceship object with a mesh of a specified size
	class spaceship
	{
	public:
		/// \brief Constructor
		spaceship(std::vector<glm::vec3> vertices);
		/// \brief Destructor
		~spaceship ();
		/// \brief Getter methods
		std::vector<glm::vec3> vertices() const { return m_vertices; }
		ref<engine::mesh> mesh() const { return m_mesh; }
		static ref<spaceship> create(std::vector<glm::vec3> vertices);
	private:
		/// \brief Fields
		// vertices of the spaceship specified by a vector of positions.
		std::vector<glm::vec3> m_vertices;
		ref<engine::mesh> m_mesh;
	};
}
