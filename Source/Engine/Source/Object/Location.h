// ◦ Xyz ◦
#pragma once

#include <Object/Resource.h>
#include <glm/glm.hpp>
#include "Model.h"

namespace Engine
{
	struct Object
	{
		glm::mat4x4 mat = glm::mat4x4(1.f);
		Model::Ptr model;
	};

	struct Glider
	{
		glm::mat4x4 mat = glm::mat4x4(1.f);
		Model::Ptr model;
	};

	class Location : public Engine::Resource<Location>
	{
	public:
		using Ptr = mystd::shared_ptr<Location>;

		Location() = delete;
		Location(std::string_view name, bool needLoad = true);

	public:
		void Load();

	private:
	public:
		std::vector<Object> _objects;
		std::vector<Glider> _gliders;
	};
}

#define LOCATIONS Engine::Location::Instance()
