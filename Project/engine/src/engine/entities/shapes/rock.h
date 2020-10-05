#pragma once

namespace engine

{
	class mesh;
	/// \brief Class creating a rock object with a mesh of a specified size
	class rock
	{
	public:
		/// \brief Constructor
		rock(std::vector<glm::vec3> vertices);
		/// \brief Destructor
		~rock();
		/// \brief Getter methods
		std::vector<glm::vec3> vertices() const { return m_vertices; }
		ref<engine::mesh> mesh() const { return m_mesh; }
		static ref<rock> create(std::vector<glm::vec3> vertices);
	private:
		/// \brief Fields
		// vertices of the rock specified by a vector of positions.
		std::vector<glm::vec3> m_vertices;
		ref<engine::mesh> m_mesh;
	};
}
