// ◦ Xyz ◦

#include "Item.h"

using namespace Engine;

Item::Item(std::string_view name)
	: _name(name)
{}

const std::string& Item::GetName() const
{
	return _name;
}

const Item::Ptr& Item::GetCurrent()
{
	if (!currentItem) {
		currentItem = MakeItem<Item>(defaultName);
	}
	return currentItem;
}

const Item::Ptr& Item::GetItem(std::string_view name, bool setSurrent)
{
	const auto it = items.find(name);
	const Item::Ptr& item = it != items.end() ? *it : MakeItem<Item>(defaultName);

	if (setSurrent) {
		currentItem = item;
	}

	return item;
}

bool Item::IsExist(std::string_view name)
{
	return items.contains(name);
}
