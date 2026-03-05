// ◦ Xyz ◦
#pragma once

#include <Log.h>
#include <string>
#include <exception>
#include <memory>
#include <mystd_memory.h>

namespace mystd::Examples
{
	using STRING = std::string;
	using STRING_VIEW = std::string_view;

	class TestClass;
	
	std::ostream& operator << (std::ostream& os, const TestClass& in);

	class TestClass final {
		friend std::ostream& operator << (std::ostream& os, const TestClass& in);

	public:
		using Ptr = std::shared_ptr<TestClass>;
		//using Ptr = mystd::shared_ptr<TestClass>;

		struct TestException final : public std::exception {
			TestException(STRING&& mess, int val, const STRING& text) : info(TO_STRING("{} [{}, {}]", std::move(mess), val, text)) {}
			char const* what()  const override {
				return info.c_str();
			}
			STRING info;
			inline static int throwCounter = 1;
		};

		TestClass();
		TestClass(const TestClass& in);
		TestClass(TestClass&& in) noexcept;
		TestClass(int val);
		TestClass(const STRING& text);
		TestClass(STRING&& text) noexcept;
		TestClass(int val, const STRING& text);
		TestClass(int val, STRING&& text) noexcept;
		~TestClass();
		int GetVal() const;
		int SetVal(int newVal);
		const STRING& GetText() const;
		const STRING& SetText(STRING_VIEW newText);

		TestClass& operator = (const TestClass& in);
		TestClass& operator = (TestClass&& in) noexcept;
		TestClass& operator = (int val);
		TestClass& operator = (const STRING& text);

		operator bool() const;
		operator int() const;
		operator const STRING& () const;

		bool operator == (const TestClass& in) const;
		bool operator == (int val) const;
		bool operator == (const STRING& text) const;

		bool operator < (const TestClass& in) const;

		TestClass& operator + (const TestClass& in);
		TestClass& operator + (TestClass&& in) noexcept;
		TestClass operator + (const TestClass& in) const;

	private:
	public:
		int _val;
		STRING _text;

		inline static bool logOn = true;
	};
}
