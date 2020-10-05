#include "pch.h"
#include "spaceship.h"
#include <engine.h>	

engine::spaceship::spaceship(std::vector<glm::vec3> vertices) : m_vertices(vertices)
{
	std::vector<glm::vec3> normals;
	// front normal
	normals.push_back(glm::cross(vertices.at(0) - vertices.at(2), vertices.at(0) - vertices.at(1)));
	// left normal
	normals.push_back(glm::cross(vertices.at(0) - vertices.at(3), vertices.at(0) - vertices.at(2)));
	// right normal
	normals.push_back(glm::cross(vertices.at(0) - vertices.at(1), vertices.at(0) - vertices.at(3)));
	// bottom normal
	normals.push_back(glm::cross(vertices.at(1) - vertices.at(2), vertices.at(1) - vertices.at(3)));

	// front normal
	normals.push_back(glm::cross(vertices.at(2) - vertices.at(0), vertices.at(1) - vertices.at(0)));
	// left normal
	normals.push_back(glm::cross(vertices.at(3) - vertices.at(0), vertices.at(2) - vertices.at(0)));
	// right normal
	normals.push_back(glm::cross(vertices.at(1) - vertices.at(0), vertices.at(3) - vertices.at(0)));
	// bottom normal
	normals.push_back(glm::cross(vertices.at(2) - vertices.at(1), vertices.at(3) - vertices.at(1)));

	std::vector<mesh::vertex> spaceship_vertices
	{
		//upper

		//front
		//  position			normal			      tex coord       
		{ vertices.at(0),		normals.at(0),		{ 0.f,  0.f } },
		{ vertices.at(2),		normals.at(0),		{ 1.f,  0.f } },
		{ vertices.at(1),		normals.at(0),		{ 0.5f, 1.f } },


		////left
		{ vertices.at(0),		normals.at(1),		{ 0.f,  0.f } },
		{ vertices.at(3),		normals.at(1),		{ 1.f,  0.f } },
		{ vertices.at(2),		normals.at(1),		{ 0.5f, 1.f } },

		////right
		{ vertices.at(0),		normals.at(2),		{ 0.f,  0.f } },
		{ vertices.at(1),		normals.at(2),		{ 1.f,  0.f } },
		{ vertices.at(3),		normals.at(2),		{ 0.5f, 1.f } },


		//lower

		//front
		{ vertices.at(4),		normals.at(0),		{ 0.f,  0.f } },
		{ vertices.at(2),		normals.at(0),		{ 1.f,  0.f } },
		{ vertices.at(3),		normals.at(0),		{ 0.5f, 1.f } },


		//left
		{ vertices.at(4),		normals.at(1),		{ 0.f,  0.f } },
		{ vertices.at(1),		normals.at(1),		{ 1.f,  0.f } },
		{ vertices.at(2),		normals.at(1),		{ 0.5f, 1.f } },

		//right
		{ vertices.at(3),		normals.at(2),		{ 0.f,  0.f } },
		{ vertices.at(1),		normals.at(2),		{ 1.f,  0.f } },
		{ vertices.at(4),		normals.at(2),		{ 0.5f, 1.f } },


	};

	const std::vector<uint32_t> spaceship_indices
	{
		//upper
		0,  1,  2,//front
		3,  4,  5,//left
		6,	7,  8,//right
		9,  10, 11,//front
		//lower
		12, 13, 14,//left
		15, 16, 17,//right


	};

	m_mesh = engine::mesh::create(spaceship_vertices, spaceship_indices);
}

engine::spaceship::~spaceship() {}

engine::ref<engine::spaceship> engine::spaceship::create(std::vector<glm::vec3> vertices)
{
	return std::make_shared<engine::spaceship>(vertices);
}
