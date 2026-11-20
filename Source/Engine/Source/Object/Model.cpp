// ◦ Xyz ◦

#include "Model.h"
#include <Files/FileManager.h>
#include <Object/Loaders/ObjLoader.h>
#include <StringUtils.h>
#include "Shape.h"
#include "Texture.h"

using namespace Engine;

Model::Model(std::string_view name, bool load, bool bindVao)
	: Resource(name)
{
	if (load) {
		Load(bindVao);
	}
}

Model::Model(std::string_view name, std::string_view fileNamePath, bool load, bool bindVao)
	: Resource(name)
{
	if (load) {
		Load(fileNamePath, bindVao);
	}
}

bool Model::Load(bool createBuffer)
{
	std::string_view shapeName = "NLO/NLO100";
	std::string_view textureName = "NLO/metallic_circle.jpg";

	_shape = SHAPES.Get(shapeName);

	if (!_shape || !_shape->Load(createBuffer)) {
		LOG_BREAK("[Model::Load] Fail load shape: {}", GetName());
		return false;
	}

	//_texture = Engine::Texture::Instance().Get(textureName);
	_texture = TEXTURES.Get(textureName);
	_texture->Load();

	return _shape->Load(createBuffer);
}

bool Model::Load(std::string_view fileNamePath, bool createBuffer)
{
	return false;
}
