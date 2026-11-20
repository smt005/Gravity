// ◦ Xyz ◦
#pragma once

#include "Resource.h"

namespace Engine
{
	class Texture final : public Resource<Texture>
	{
	public:
		using Ptr = mystd::shared_ptr<Texture>;

		Texture() = default;
		Texture(std::string_view name, bool needLoad = true);
		~Texture();

		unsigned int Id();
		bool Load();
		void Save(std::string_view fileNamePath) const;

	private:
		unsigned int _id = 0;
		unsigned int _width = 0;
		unsigned int _height = 0;

	};
}

#define TEXTURES Engine::Texture::Instance()
