// ◦ Xyz ◦

#include "Model.h"
#include <Files/FileManager.h>
#include <StringUtils.h>
#include <Common/Json.h>
#include <Logs.h>
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

bool Model::Load(bool createBuffer)
{
	static const std::filesystem::path fileNamePath = "Models/Base.json";
	static Engine::Json jsonDataBase(fileNamePath);

	const std::string shapeName = jsonDataBase[GetName(), "shape"];
	const std::string textureName = jsonDataBase[GetName(), "texture"];

	_shape = SHAPES.Get(shapeName);
	if (!_shape || !_shape->Load(createBuffer)) {
		LOG_BREAK("[Model::Load] Fail load shape: {}", GetName());
		return false;
	}

	_texture = TEXTURES.Get(textureName);
	if (!_texture || !_texture->Load()) {
		LOG_BREAK("[Model::Load] Fail load texture: {}", GetName());
		return false;
	}

	return true;
}
