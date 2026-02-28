// ◦ Xyz ◦
#pragma once

#include <string>
#include <memory>
#include <unordered_set>
#include <exception>
#include <Log.h>

namespace Engine
{
	class Item {
	public:
		using Ptr = std::shared_ptr<Item>;

		Item() = delete;
		Item(std::string_view name);
		virtual ~Item() = default;
		const std::string& GetName() const;

	private:
		std::string _name;

	public:
		static const Item::Ptr& GetCurrent();
		static const Ptr& GetItem(std::string_view name, bool setSurrent = false);
		static bool IsExist(std::string_view name);

		template <typename T, typename ...Args>
		static const Ptr& MakeItem(std::string_view name, Args&&... args)
		{
			const auto it = items.find(name);
			if (it != items.end()) {
				return *it;
			}

			const std::shared_ptr<T> newItem = std::make_shared<T>(name, std::forward<Args>(args)...);
			items.emplace(newItem);
			return newItem;
		}

	private:
		struct Hash {
			using is_transparent = void;

			size_t operator()(const Ptr& item) const noexcept {
				return std::hash<std::string_view>{}(item->_name);
			}
			size_t operator()(std::string_view itemName) const noexcept {
				return std::hash<std::string_view>{}(itemName);
			}
		};
		struct IsEqual {
			using is_transparent = void;

			bool operator()(const Ptr& left, const Ptr& right) const noexcept {
				return left->_name == right->_name;
			}
			bool operator()(std::string_view leftName, const Ptr& right) const noexcept {
				return leftName == right->_name;
			}
			bool operator()(const Ptr& left, const std::string_view rightName) const noexcept {
				return left->_name == rightName;
			}
		};

	private:
		inline static std::string_view defaultName = "##default";
		inline static Item::Ptr currentItem;
		inline static std::unordered_set<Item::Ptr, Hash, IsEqual> items;
	};
}
