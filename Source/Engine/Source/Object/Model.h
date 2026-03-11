// ◦ Xyz ◦
#pragma once

#include <Object/Mesh.h>
#include <Object/Shape.h>
#include <Object/Texture.h>
#include <Object/Resource.h>

namespace Engine
{
	class Model final : public Resource<Model>
	{
	public:
		using Ptr = mystd::shared_ptr<Model>;

		Model() = default;
		Model(std::string_view name, bool load = true, bool bindVao = false);
		Model(std::string_view name, std::string_view fileNamePath, bool load = false, bool bindVao = false);
		~Model() = default;

		bool Load(bool createBuffer = false);
		bool Load(std::string_view fileNamePath, bool createBuffer = false);

	private:
	public:
		Shape shape;
		Texture texture;
	};
}

#define MODELS Engine::Model::Instance()
