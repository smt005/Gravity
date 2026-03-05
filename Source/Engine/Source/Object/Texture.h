// ◦ Xyz ◦
#pragma once

#include "Resource.h"

namespace Engine
{
	class Texture final : public Resource<Texture>
	{
	public:
		using Ptr = std::shared_ptr<Texture>;

	private:
		std::string _fileName;
		unsigned int _id = 0;
		unsigned int _width = 0;
		unsigned int _height = 0;

	public:
		Texture() = delete;
		Texture(std::string_view name, bool needLoad = true);
		~Texture();

		unsigned int Id();
		unsigned int Load();

		void SetId(const unsigned int id);

	public:
		static unsigned int LoadTexture(const char* fileName);
		static unsigned int LoadTexture(const std::string& fileName, unsigned int& width, unsigned int& height);
	};
}
