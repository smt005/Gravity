// ◦ Xyz ◦

#include "Texture.h"
#include "glad/gl.h"
#include "FreeImage.h"
#include <FileManager/FileManager.h>

using namespace Engine;

Texture::~Texture()
{
	glDeleteTextures(1, &_id);
}

Texture::Texture(std::string_view name, bool needLoad)
	: Resource(name)
{
	if (needLoad) {
		Load();
	}
}

unsigned int Texture::Id()
{
	if (_id != 0) return _id;
	_id = Load();
	return _id;
}

bool Texture::Load()
{
	const std::filesystem::path fullFileName = Engine::FileManager::Get("base").GetRoot() / "Textures" / GetName();
	if (!std::filesystem::exists(fullFileName)) {
		return false;
	}

	FREE_IMAGE_FORMAT fif = FreeImage_GetFileType(fullFileName.string().c_str(), 0);
	if (fif == FIF_UNKNOWN) {
		fif = FreeImage_GetFIFFromFilename(fullFileName.string().c_str());
	}

	if (fif == FIF_UNKNOWN) {
		return false;
	}

	FIBITMAP* bitMap = nullptr;
	if (FreeImage_FIFSupportsReading(fif)) {
		bitMap = FreeImage_Load(fif, fullFileName.string().c_str());
	}

	if (!bitMap) {
		return false;
	}

	unsigned int width = FreeImage_GetWidth(bitMap);
	unsigned int height = FreeImage_GetHeight(bitMap);
	BYTE* bytes = FreeImage_GetBits(bitMap);

	if ((bytes == 0) || (width == 0) || (height == 0)) {
		width = 0;
		height = 0;
		FreeImage_Unload(bitMap);
		return false;
	}

	unsigned int textureId = 0;
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);

	if (FreeImage_GetBPP(bitMap) == 32) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, bytes);
	}
	else if (FreeImage_GetBPP(bitMap) == 24) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, bytes);
	}
	else {
		FreeImage_Unload(bitMap);
		width = 0;
		height = 0;
		return false;
	}

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	FreeImage_Unload(bitMap);

	_id = textureId;
	_width = width;
	_height = height;

	return true;
}
