// ◦ Xyz ◦
#pragma once

#include <memory>
#include <unordered_set>
#include <string>
#include <ostream>

namespace Engine
{
	/* Example
	class Resource final : public Storage<Resource>
	{
	public:
		Resource(std::string_view name)
			: _name(name)
		{}

		std::string_view Name() const {
			return _name;
		}

	private:
		std::string _name;
	};
	*/

	template <typename T>
	class Storage
	{
	public:
		using Ptr = std::shared_ptr<T>;

	private:
		struct Hash {
			using is_transparent = void;

			size_t operator()(std::string_view name) const noexcept {
				return std::hash<std::string_view>{}(name);
			}
			size_t operator()(const Ptr& ptr) const noexcept {
				return ptr ? std::hash<std::string_view>{}(ptr->Name()) : 0;
			}
		};

		struct IsEqual {
			using is_transparent = void;

			bool operator()(const Ptr& left, const Ptr& right) const noexcept {
				return (!left && !right || left.get() == right.get());
			}
			bool operator()(std::string_view leftName, const Ptr& right) const noexcept {
				return leftName == (right ? right->Name() : std::string_view());
			}
			bool operator()(const Ptr& left, std::string_view rightName) const noexcept {
				return (left ? left->Name() : std::string_view()) == rightName;
			}
		};

	public:
		using StorageClass = std::unordered_set<Ptr, Hash, IsEqual>;

		template <typename ...Args>
		inline static T& operator[](Args&&... args) {
			return *Get(std::forward<Args>(args)...);
		}

		template <typename ...Args>
		inline static T& GetRef(Args&&... args) {
			return *Get(std::forward<Args>(args)...);
		}

		template <typename ...Args>
		static const Ptr& Get(std::string_view name, Args&&... args) {
			auto it = data.find(name);
			if (it == data.end()) {
				const auto emplaceResult = data.emplace(std::make_shared<T>(name, std::forward<Args>(args)...));
				if (!emplaceResult.second || !*emplaceResult.first) {
					throw std::logic_error(TO_STRING("[{}::Storage::Get] nullptr by name: {}", typeid(T).name(), name));
				}

				it = emplaceResult.first;
			}

			return *it;
		}

		template <typename Name>
		static void Erase(Name&& name) {
			data.erase(std::forward<Name>(name));
		}

		//static void Erase(const Ptr& ptr) {
			//data.erase(ptr);
		//}

		static bool Contains(std::string_view name) {
			return data.contains(name);
		}

		static StorageClass& Data() {
			return data;
		}

		static Storage<T>& Ins() {
			static Storage<T> instance;
			return instance;
		}

	private:
		inline static StorageClass data;
	};

	template <typename T>
	static std::ostream& operator << (std::ostream& os, const Storage<T>& storage)
	{
		const Storage<T>::StorageClass& data = storage.Data();
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
