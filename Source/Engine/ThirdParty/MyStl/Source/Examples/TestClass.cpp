// ◦ Xyz ◦

#include "TestClass.h"

using namespace mystd::Examples;

std::ostream& mystd::Examples::operator << (std::ostream& os, const TestClass& in)
{
	os << '{' << in._val << ",'" << in._text << "'}";
	return os;
}

TestClass::TestClass() : _val(0)
{
	if (logOn) LOG("TestClass &{}", reinterpret_cast<std::uintptr_t>(this));
}

TestClass::TestClass(const TestClass& in) : _val(in._val), _text(in._text)
{
	if (TestException::throwCounter-- == 0) {
		//LOG("THROW TestClass(const TestClass& in &{}", reinterpret_cast<std::uintptr_t>(this));
		//throw(TO_STRING("THROW TestClass(const TestClass& in &{}", reinterpret_cast<std::uintptr_t>(this)));
		//throw (exception(TO_STRING("THROW TestClass(const TestClass& in &{}", reinterpret_cast<std::uintptr_t>(this)).c_str()));
		//throw(   TestException(  TO_STRING("THROW TestClass(const TestClass& in &{}", reinterpret_cast<std::uintptr_t>(this)), _val, _text  )   );
	}

	if (logOn) LOG("TestClass(const TestClass&): {{}, {}} &{} << ({})", in._val, in._text, reinterpret_cast<std::uintptr_t>(this), reinterpret_cast<std::uintptr_t>(&in));
}

TestClass::TestClass(TestClass&& in) noexcept : _val(in._val), _text(std::move(in._text))
{
	if (logOn) LOG("TestClass(TestClass&&): {{}, {}} &{} << ({})", _val, _text, reinterpret_cast<std::uintptr_t>(this), reinterpret_cast<std::uintptr_t>(&in));
	in._val = -1;
	in._text = "null";
}

TestClass::TestClass(int val) : _val(val) {
	if (logOn) LOG("TestClass(int): {} &{}", _val, reinterpret_cast<std::uintptr_t>(this));
}

TestClass::TestClass(const STRING& text) : _val(0), _text(text)
{
	if (logOn) LOG("TestClass(const string&): {} &{}", _text, reinterpret_cast<std::uintptr_t>(this));
}

TestClass::TestClass(STRING&& text) noexcept : _val(0), _text(std::move(text))
{
	if (logOn) LOG("TestClass(string&&): {} &{}", _text, reinterpret_cast<std::uintptr_t>(this));
}

TestClass::TestClass(int val, const STRING& text) : _val(val), _text(text)
{
	if (logOn) LOG("TestClass(int, const string&): {}, {} &{}", _val, _text, reinterpret_cast<std::uintptr_t>(this));
}

TestClass::TestClass(int val, STRING&& text) noexcept : _val(val), _text(std::move(text))
{
	if (logOn) LOG("TestClass(int, string&&): {}, {} &{}", _val, _text, reinterpret_cast<std::uintptr_t>(this));
}

TestClass::~TestClass()
{
	if (logOn) LOG("~TestClass: {}, {} &{} DESTRUCTOR", _val, _text, reinterpret_cast<std::uintptr_t>(this));
}

int TestClass::GetVal() const
{
	if (logOn) LOG("TestClass int GetVal(): {} &{}", _val, reinterpret_cast<std::uintptr_t>(this));
	return _val;
}

int TestClass::SetVal(int newVal)
{
	if (logOn) LOG("TestClass int SetVal(int): {} -> {} &{}", _val, newVal, reinterpret_cast<std::uintptr_t>(this));
	_val = newVal;
	return _val;
}

const STRING& TestClass::GetText() const
{
	if (logOn) LOG("TestClass STRING& GetText(): {} &{}", _text, reinterpret_cast<std::uintptr_t>(this));
	return _text;
}

const STRING& TestClass::SetText(STRING_VIEW newText)
{
	if (logOn) LOG("TestClass STRING& SetText(STRING_VIEW): {} -> {} &{}", _text, newText, reinterpret_cast<std::uintptr_t>(this));
	_text = newText;
	return _text;
}

TestClass& TestClass::operator = (const TestClass& in)
{
	if (logOn) LOG("TestClass operator = (const TestClass&): {{}, {}} -> {{}, {}} &{} << ({})", _val, _text, in._val, in._text, reinterpret_cast<std::uintptr_t>(this), reinterpret_cast<std::uintptr_t>(&in));
	_val = in._val;
	_text = in._text;
	return *this;
}

TestClass& TestClass::operator = (TestClass&& in) noexcept
{
	if (logOn) LOG("TestClass operator = (TestClass&&): {{}, {}} -> {{}, {}} &{} << ({})", _val, _text, in._val, in._text, reinterpret_cast<std::uintptr_t>(this), reinterpret_cast<std::uintptr_t>(&in));
	_val = in._val;
	in._val = -1;
	_text = in._text;
	in._text= "null";
	return *this;
}

TestClass& TestClass::operator = (int val)
{
	if (logOn) LOG("TestClass operator = (int): {} -> {} &{}", _val, val, reinterpret_cast<std::uintptr_t>(this));
	_val = val;
	return *this;
}

TestClass& TestClass::operator = (const STRING& text)
{
	if (logOn) LOG("TestClass operator = (const string&: {} -> {} &{}", _text, text, reinterpret_cast<std::uintptr_t>(this));
	_text = text;
	return *this;
}

TestClass::operator bool() const
{
	return _val != 0 || !_text.empty();
}

TestClass::operator int() const
{
	//if (logOn) LOG("TestClass operator int: {} &{}", _val, reinterpret_cast<std::uintptr_t>(this));
	//return _val;
	int val = _val + _text.size();
	if (logOn) LOG("TestClass operator int: {} &{}", val, reinterpret_cast<std::uintptr_t>(this));
	return val;
}

TestClass::operator const STRING&() const
{
	if (logOn) LOG("TestClass operator const string&: {} &{}", _text, reinterpret_cast<std::uintptr_t>(this));
	return _text;
}

bool TestClass::operator == (const TestClass& in) const
{
	return _val == in._val && _text == in._text;
}

bool TestClass::operator == (int val) const
{
	return _val == val;
}

bool TestClass::operator == (const STRING& text) const
{
	return _text == text;
}

bool TestClass::operator < (const TestClass& in) const
{
	if (_val != in._val) {
		return _val < in._val;
	}
	
	return std::lexicographical_compare(_text.begin(), _text.end(), in._text.begin(), in._text.end());
}

TestClass& TestClass::operator + (const TestClass& in)
{
	_val += in._val;
	_text = in._text;
	return *this;
}

TestClass TestClass::operator + (const TestClass& in) const
{
	return { _val + in._val, _text + in._text };
}

TestClass& TestClass::operator + (TestClass&& in) noexcept
{
	_val += in._val;
	in._val = -1;
	std::swap(_text, in._text);
	in._text = "null";
	return *this;
}
