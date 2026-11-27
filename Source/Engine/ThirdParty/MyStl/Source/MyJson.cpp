// ◦ Xyz ◦

#include "MyJson.h"

#include "../../../Source/SpecificationLogs/LogSpecification.h"
#include "../../../Source/SpecificationLogs/LogMyStlSpecification.h"
#include "../../../Source/SpecificationLogs/LogStlSpecification.h"
#include "Log.h"

namespace mystd
{
	MyJson::MyJson(std::string_view dataStr)
	{
		Parse(dataStr);
	}

	void MyJson::Parse(std::string_view dataStr)
	{}
}

//...

namespace _TestMyJson_
{

	template <typename T>
	void SizePrint()
	{
		PRINT("{}: {}", typeid(T).name(), sizeof(T));
	}

	template <typename T>
	void PrintInfo(T&& value)
	{
		//PRINT("{}: {} = {}", typeid(T).name(), sizeof(T), value);
		PRINT("{}", value);
	}

}

void mystd::TestJsonMystd()
{
	using namespace _TestMyJson_;

	/*{
		StructTestReturn s;
		{
			int v = s;
			PrintInfo(v);
		}
		{
			std::string v = s;
			PrintInfo(v);
		}
	}*/

	/*{
		PRINT("Size print\n");

		SizePrint<bool>();
		SizePrint<int>();
		SizePrint<double>();
		SizePrint<long long>();
		SizePrint<size_t>();
		//SizePrint<AnyData>();
		SizePrint<std::any>();
		SizePrint<std::unique_ptr<std::any>>();
		SizePrint<Variant>();

		PRINT("Size print end\n");
	}*/

	{
		MyJson myJson;

		{
			int value = 9;
			myJson = value;

			int newValue = myJson;
			PrintInfo(newValue);
		}

		{
			std::string str = "Text";
			myJson = str;

			std::string newValue = myJson;
			PrintInfo(newValue);
		}
		{
			std::string str = "Text";
			myJson = str;

			const std::string& newValue = myJson;
			PrintInfo(newValue);
		}

		{
			std::vector<std::string> arr = { "012", "345", "678" };
			myJson = arr;

			std::vector<std::string> newValue = myJson;
			PrintInfo(newValue);
		}
		{
			std::vector<std::string> arr = { "987", "654", "321" };
			myJson = arr;

			const std::vector<std::string>& newValue = myJson;
			PrintInfo(newValue);
		}
	}
}
