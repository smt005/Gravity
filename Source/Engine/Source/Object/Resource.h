// ◦ Xyz ◦
#pragma once

#include <memory>
#include <mystd_memory.h>
#include <unordered_set>
#include <string>
#include <ostream>
#include <Singletone.h>
#include <Common/Common.h>

namespace Engine
{
	/* Пример класса для использования Resource
	class ExampleClass : public Resource<ExampleClass>
	{
	public:
		ExampleClass = delete;
		ExampleClass(std::string_view name)
			: Resource::_name(name)
		{}
	};
	*/

	template <typename T>
	class Resource : public mystd::Singletone<Resource<T>>
	{
		friend mystd::Singletone<Resource<T>>;

		Resource() = default;
		Resource(const Resource&) = delete;
		Resource(Resource&&) = delete;
		Resource& operator = (const Resource&) = delete;
		Resource& operator = (Resource&&) = delete;

	protected:
		Resource(std::string_view name) : _name(name) {}
		~Resource() = default;

	public:
		using Ptr = mystd::shared_ptr<T>;

		const std::string& GetName() const {
			return _name;
		}

	private:
		std::string _name;

		struct Hash {
			using is_transparent = void;

			size_t operator()(std::string_view name) const noexcept {
				return std::hash<std::string_view>{}(name);
			}
			size_t operator()(const Ptr& ptr) const noexcept {
				return ptr ? std::hash<std::string_view>{}(ptr->_name) : 0;
			}
		};

		struct IsEqual {
			using is_transparent = void;

			bool operator()(const Ptr& left, const Ptr& right) const noexcept {
				return (left.get() == right.get());
			}
			bool operator()(std::string_view leftName, const Ptr& right) const noexcept {
				return leftName == (right ? right->_name : std::string_view());
			}
			bool operator()(const Ptr& left, std::string_view rightName) const noexcept {
				return (left ? left->_name : std::string_view()) == rightName;
			}
		};

	public:
		using ResourceContainer = std::unordered_set<Ptr, Hash, IsEqual>;

		template <typename ...Args>
		static T& operator[](Args&&... args) {
			return *Get(std::forward<Args>(args)...);
		}

		template <typename ...Args>
		static T& GetRef(Args&&... args) {
			return *Get(std::forward<Args>(args)...);
		}

		template <typename ...Args>
		static const Ptr& Get(std::string_view name, Args&&... args) {
			auto it = data.find(name);
			if (it == data.end()) {
				const auto emplaceResult = data.emplace(mystd::make_shared<T>(name, std::forward<Args>(args)...));
				if (!emplaceResult.second || !*emplaceResult.first) {
					throw std::logic_error(TO_STRING("[{}::Resource::Get] nullptr by name: {}", typeid(T).name(), name));
				}

				it = emplaceResult.first;
			}

			return *it;
		}

		template <typename Name>
		static void Erase(Name&& name) {
			data.erase(std::forward<Name>(name));
		}

		static bool Contains(std::string_view name) {
			return data.contains(name);
		}

		static const ResourceContainer& Data() {
			return data;
		}

	private:
		inline static ResourceContainer data;
	};

	template <typename T>
	static std::ostream& operator << (std::ostream& os, const Resource<T>& storage)
	{
		const Resource<T>::ResourceContainer& data = storage.Data();
		const size_t size = data.size();
		size_t number = 0;
		os << '[';

		for (const auto& item : data) {
			os << item->Name();
			if (++number != size) {
				os << ',';
			}
		}

		os << ']';
		return os;
	}
}
