// ◦ Xyz ◦

#include "Model.h"
#include <FileManager/FileManager.h>
#include <Object/Loaders/ObjLoader.h>
#include <StringUtils.h>

using namespace Engine;

Shape::Shape(std::string_view name, bool load, bool bindVao)
	: Resource(name)
{
	if (load) {
		Load(bindVao);
	}
}

Shape::Shape(std::string_view name, std::string_view fileNamePath, bool load, bool bindVao)
	: Resource(name)
{
	if (load) {
		Load(fileNamePath, bindVao);
	}
}

bool Shape::Load(bool createBuffer)
{
	return false;
}

bool Shape::Load(std::string_view fileNamePath, bool createBuffer)
{
	return false;
}
