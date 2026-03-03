// ◦ Xyz ◦

#include "Shape.h"
#include <FileManager/FileManager.h>
#include <Object/Loaders/ObjLoader.h>
#include <Log.h>

using namespace Engine;

Shape::Shape(std::string_view name, bool load, bool bindVao)
	: _name(name)
{
	if (load) {
		Load(bindVao);
	}
}

Shape::Shape(std::string_view name, std::string_view fileNamePath, bool load, bool bindVao)
{
	if (load) {
		Load(fileNamePath, bindVao);
	}
}

bool Shape::Load(bool createBuffer)
{
	const std::string fileNamePath = TO_STRING("Shapes/{}.obj", _name);
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
