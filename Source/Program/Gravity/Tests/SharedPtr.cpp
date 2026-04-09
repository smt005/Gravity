// ◦ Xyz ◦

#include "Test.h"
#include <vector>
#include <memory>
#include <Common/Common.h>
#include <Logs.h>

namespace
{
	class Aclass
	{
	public:
		using Ptr = std::shared_ptr<Aclass>;

		Aclass() {
			LOG("class: {}", typeid(*this).name());
		}
		~Aclass() {
			LOG("~class: {}", typeid(*this).name());
		}
		virtual void Print() {
			LOG("Print<{}>: {}", typeid(*this).name(), aValue);
		}
		int aValue = std::rand() % 1000;

		static void SetClass(Ptr _classPtr) {
			classPtr = _classPtr;
		}

		inline static Ptr classPtr;
	};

	class Bclass : public Aclass
	{
	public:
		using Ptr = std::shared_ptr<Bclass>;

		Bclass() {
			LOG("class: {}", typeid(*this).name());
		}
		~Bclass() {
			LOG("~class: {}", typeid(*this).name());
		}
		int bValue = std::rand() % 1000;
	};
}

void Tests::SharedPtr()
{
	std::vector<Aclass::Ptr> arr;
	
	{
		LOG("// Aclass ..................................");

		arr.emplace_back(std::make_shared<Aclass>());
		arr.emplace_back(std::make_shared<Aclass>());
		arr.emplace_back(std::make_shared<Aclass>());

		Aclass::classPtr = arr.front();
	}
	{
		LOG("// Bclass ..................................");

		arr.emplace_back(std::make_shared<Bclass>());
		arr.emplace_back(std::make_shared<Bclass>());
		arr.emplace_back(std::make_shared<Bclass>());
	}

	LOG("-----------------------------------");

	for (Aclass::Ptr& item : arr) {
		item->Print();
	}

	LOG("////////////////////////////////////////");

	Aclass::classPtr->Print();
	//Aclass::classPtr = arr.back();
	Aclass::SetClass(arr.back());
	Aclass::classPtr->Print();

	LOG("\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\");

	{
		size_t size = arr.size();
		arr[size - 1] = arr[0];
		arr[size - 2] = arr[1];
	}

	for (Aclass::Ptr& item : arr) {
		item->Print();
	}

	LOG("-----------------------------------");
}
