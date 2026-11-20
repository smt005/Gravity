// ◦ Xyz ◦

#include "Model.h"
#include <stdexcept>
#include <Files/FileManager.h>
#include <Object/Loaders/ObjLoader.h>
#include <StringUtils.h>
#include <Common/JsonHelper.h>
#include <nlohmann/json.hpp>
#include <Logs.h>
#include "Shape.h"
#include "Texture.h"

using namespace Engine;
using Json = nlohmann::json;

static Json& GetJsonDataBase(const std::string& modelName)
{
	static const std::string fileNamePath = "Models/Base.json";
	static Json jsonDataBase;

	if (jsonDataBase.empty()) {
		jsonDataBase = Engine::LoadJson(fileNamePath, "base");

		if (jsonDataBase.empty()) {
			throw std::logic_error(TO_STRING("[Model] GetJsonDataBase, data base '{}' not load.", fileNamePath));
		}
	}

	if (!jsonDataBase.contains(modelName.c_str()) || !jsonDataBase.at(modelName.c_str()).is_object()) {
		throw std::logic_error(TO_STRING("[Model] GetJsonDataBase, data base moel '{}' not object.", modelName));
	}

	return jsonDataBase[modelName];
}

Model::Model(std::string_view name, bool load, bool bindVao)
	: Resource(name)
{
	if (load) {
		Load(bindVao);
	}
}

/*Model::Model(std::string_view name, std::string_view fileNamePath, bool load, bool bindVao)
	: Resource(name)
{
	if (load) {
		Load(fileNamePath, bindVao);
	}
}*/

bool Model::Load(bool createBuffer)
{
	const std::string shapeName = GetJsonValue<std::string>("shape", GetJsonDataBase(GetName()));
	const std::string textureName = GetJsonValue<std::string>("texture", GetJsonDataBase(GetName()));

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

/*bool Model::Load(std::string_view fileNamePath, bool createBuffer)
{
	return false;
}*/
