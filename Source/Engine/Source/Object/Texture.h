// ◦ Xyz ◦
#pragma once

#include "Resource.h"

namespace Engine
{
	class Texture final : public Resource<Texture>
	{
	public:
		using Ptr = std::shared_ptr<Texture>;

		Texture() = delete;
		Texture(std::string_view name, bool needLoad = true);
		~Texture();

		unsigned int Id();
		bool Load();

	private:
		unsigned int _id = 0;
		unsigned int _width = 0;
		unsigned int _height = 0;

	};
}
