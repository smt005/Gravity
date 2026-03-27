// ◦ Xyz ◦
#pragma once

#include <string>
#include <random>
#include <memory>
#include <list>
#include <mystd_memory.h>

namespace Engine
{
	bool IsDebugging();

	template <typename T>
	T Random(T min, T max)
	{
		static std::mt19937 rng{ std::random_device{}() };
		std::uniform_real_distribution<T> dist(min, max);
		return dist(rng);
	}

	template <typename Arg, typename ...Args>
	Arg&& FirstArg(Arg&& arg, Args&&...) {
		return std::forward<Arg>(arg);
	}

	static std::string ExtractClassName(const std::string_view fullName)
	{
		std::string result;
		size_t pos = fullName.npos;

		// TODO:
		{
			const std::string_view text = "::";
			pos = fullName.find_last_of(text);
			if (pos != fullName.npos) {
				pos += text.length();
			}
		}

		if (pos == fullName.npos) {
			const std::string_view text = "class ";
			pos = fullName.find(text);

			if (pos != fullName.npos) {
				pos += text.length();
			}
		}

		if (pos == fullName.npos) {
			const std::string_view text = "struct ";
			pos = fullName.find(text);

			if (pos != fullName.npos) {
				pos += text.length();
			}
		}

		if (pos != fullName.npos) {
			result = std::string(fullName.substr(pos, fullName.size() - pos));
		}
		else {
			result = std::string(fullName);
		}

		return result;
	}

	template <typename T>
	std::string GetClassName()
	{
		const std::string_view typeName = typeid(T).name();
		return ExtractClassName(typeName);
	}

	class VariableHandler final {
	private:
		struct Variable {
			// TODO: Реалезовать удалятор у mystd::shared
			virtual ~Variable() = default;
		};

		using Ptr = std::shared_ptr<Variable>;

		template <typename T>
		struct VariableT final : Variable {
			T& _ref;
			T _prevValue;
			VariableT(T& ref, T value)
				: _ref(ref)
				, _prevValue(std::move(_ref))
			{
				_ref = std::forward<T>(value);
			}
			~VariableT() {
				_ref = std::move(_prevValue);
			}
		};

		VariableHandler(const VariableHandler&) = delete;
		VariableHandler(VariableHandler&&) = delete;
		void operator = (const VariableHandler&) = delete;
		void operator = (VariableHandler&&) = delete;

	public:
		VariableHandler() = default;
		~VariableHandler() = default;

		template <typename... Args>
		VariableHandler(Args&&... args) {
			Push(std::forward<Args>(args)...);
		}

		template <typename T>
		void Push(T& ref, T value) {
			auto var = std::make_shared<VariableT<T>>(ref, value);
			_refs.emplace_back(var);
		}

		template <typename T, typename... Args>
		void Push(T& ref, T value, Args&&... args) {
			auto var = std::make_shared<VariableT<T>>(ref, value);
			_refs.emplace_back(var);
			Push(std::forward<Args>(args)...);
		}

		std::list<Ptr> _refs;
	};
}
