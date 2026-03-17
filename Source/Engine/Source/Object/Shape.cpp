// ◦ Xyz ◦

#include "Shape.h"
#include <Files/FileManager.h>
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
	const std::string fileNamePath = TO_STRING("Shapes/{}.obj", GetName());
	return Load(fileNamePath, createBuffer);
}

bool Shape::Load(std::string_view fileNamePath, bool createBuffer)
{
	const std::string data = FileManager::Get("base").ReadTextFile(fileNamePath);
    mesh = Loader::ObjLoad(data);

	if (mesh.Count() == 0) {
		return false;
	}

	if (createBuffer) {
		mesh.BindBuffers();
	}

	return true;
}
