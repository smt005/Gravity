// ◦ Xyz ◦
#pragma once

#include <string>

namespace Engine
{
	template<typename T>
	std::string GetClassName()
	{
		const std::string_view typeName = typeid(T).name();
		const size_t pos = typeName.find_last_of("::") + 1;

		if (pos != typeName.npos) {
			return std::string(typeName.substr(pos, typeName.size() - pos));
		}

		return std::string(typeName);
	}

	template<typename T>
	std::string GetClassName(T*)
	{
		return GetClassName<T>();
	}

}
