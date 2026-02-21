#pragma once

#include <iostream>
#include <sstream>
#include <initializer_list>
#include <array>
#include <vector>
#include <list>
#include <forward_list>
#include <deque>
#include <queue>
#include <stack>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <exception>

namespace mystd
{
	std::ostream& operator << (std::ostream& os, const std::exception& exc)
	{
		os << "EXCEPTION: " << typeid(exc).name() << ": " << exc.what();
		return os;
	}

	std::ostream& operator << (std::ostream& os, std::byte byte)
	{
		os << static_cast<unsigned char>(byte);
		return os;
	}

	// [value]
	template <typename T>
	std::ostream& operator << (std::ostream& os, const std::initializer_list<T>& container)
	{
		const size_t size = container.size();
		size_t number = 0;
		os << '[';
		for (auto&& item : container) {
			os << item;
			if (++number != size) {
				os << ',';
			}
		}
		os << ']';
		return os;
	}

	/*template <typename T, std::size_t Count>
	std::ostream& operator << (std::ostream& os, const std::array<T, Count>& container)
	{
		const size_t size = container.size();
		size_t number = 0;
		os << '[';
		for (auto&& item : container) {
			os << item;
			if (++number != size) {
				os << ',';
			}
		}
		os << ']';
		return os;

	}*/

	template <typename T>
	std::ostream& operator << (std::ostream &os, const std::vector<T>& container)
	{
		const size_t size = container.size();
		size_t number = 0;
		os << '[';
		for (auto&& item: container) {
			os << item;
			if (++number != size) {
				os << ',';
			}
		}
		os << ']';
		return os;
	}

	template <typename T>
	std::ostream& operator << (std::ostream &os, const std::list<T>& container)
	{
		const size_t size = container.size();
		size_t number = 0;
		os << '[';
		for (auto&& item: container) {
			os << item;
			if (++number != size) {
				os << ',';
			}
		}
		os << ']';
		return os;
	}

	template <typename T>
	std::ostream& operator << (std::ostream& os, const std::forward_list<T>& container)
	{
		const size_t size = distance(container.begin(), container.end());
		size_t number = 0;
		os << '[';
		for (auto&& item : container) {
			os << item;
			if (++number != size) {
				os << ',';
			}
		}
		os << ']';
		return os;
	}

	template <typename T>
	std::ostream& operator << (std::ostream& os, const std::set<T>& container)
	{
		const size_t size = container.size();
		size_t number = 0;
		os << '[';
		for (auto&& item : container) {
			os << item;
			if (++number != size) {
				os << ',';
			}
		}
		os << ']';
		return os;
	}

	template <typename T>
	std::ostream& operator << (std::ostream& os, const std::multiset<T>& container)
	{
		const size_t size = container.size();
		size_t number = 0;
		os << '[';
		for (auto&& item : container) {
			os << item;
			if (++number != size) {
				os << ',';
			}
		}
		os << ']';
		return os;
	}

	template <typename T>
	std::ostream& operator << (std::ostream& os, const std::unordered_set<T>& container)
	{
		const size_t size = container.size();
		size_t number = 0;
		os << '[';
		for (auto&& item : container) {
			os << item;
			if (++number != size) {
				os << ',';
			}
		}
		os << ']';
		return os;
	}

	template <typename T>
	std::ostream& operator << (std::ostream& os, const std::unordered_multiset<T>& container)
	{
		const size_t size = container.size();
		size_t number = 0;
		os << '[';
		for (auto&& item : container) {
			os << item;
			if (++number != size) {
				os << ',';
			}
		}
		os << ']';
		return os;
	}

	template <typename T>
	std::ostream& operator << (std::ostream& os, const std::deque<T>& container)
	{
		const size_t size = container.size();
		size_t number = 0;
		os << '[';
		for (auto&& item : container) {
			os << item;
			if (++number != size) {
				os << ',';
			}
		}
		os << ']';
		return os;
	}

	template <typename T>
	std::ostream& operator << (std::ostream& os, const std::queue<T>& container)
	{
		auto copyContainer = container;

		const size_t size = copyContainer.size();
		size_t number = 0;
		os << '[';
		while (!copyContainer.empty()) {
			os << copyContainer.front();
			copyContainer.pop();
			if (++number != size) {
				os << ',';
			}
		}
		os << ']';
		return os;
	}

	template <typename T>
	std::ostream& operator << (std::ostream& os, const std::stack<T>& container)
	{
		auto copyContainer = container;

		const size_t size = copyContainer.size();
		std::vector<T> tempContainer(size);

		int i = size - 1;
		while (!copyContainer.empty()) {
			tempContainer[i--] = copyContainer.top();
			copyContainer.pop();
		}

		size_t number = 0;
		os << '[';
		for (auto&& item : tempContainer) {
			os << item;
			if (++number != size) {
				os << ',';
			}
		}
		os << ']';
		return os;
	}

	template <typename T>
	std::ostream& operator << (std::ostream& os, const std::priority_queue<T>& container)
	{
		auto copyContainer = container;

		const size_t size = copyContainer.size();
		std::vector<T> tempContainer(size);

		int i = size - 1;
		while (!copyContainer.empty()) {
			tempContainer[i--] = copyContainer.top();
			copyContainer.pop();
		}

		size_t number = 0;
		os << '[';
		for (auto&& item : tempContainer) {
			os << item;
			if (++number != size) {
				os << ',';
			}
		}
		os << ']';
		return os;
	}

	// [key, value]
	template <typename Tkey, typename Tvalue>
	std::ostream& operator << (std::ostream& os, const std::map<Tkey, Tvalue>& container)
	{
		const size_t size = container.size();
		size_t number = 0;
		os << '[';
		for (auto&& [key, value] : container) {
			os << key << ':' << value;
			if (++number != size) {
				os << ',';
			}
		}
		os << ']';
		return os;
	}

	template <typename Tkey, typename Tvalue>
	std::ostream& operator << (std::ostream& os, const std::multimap<Tkey, Tvalue>& container)
	{
		const size_t size = container.size();
		size_t number = 0;
		os << '[';
		for (auto&& [key, value] : container) {
			os << key << ':' << value;
			if (++number != size) {
				os << ',';
			}
		}
		os << ']';
		return os;
	}

	template <typename Tkey, typename Tvalue>
	std::ostream& operator << (std::ostream& os, const std::unordered_map<Tkey, Tvalue>& container)
	{
		const size_t size = container.size();
		size_t number = 0;
		os << '[';
		for (auto&& [key, value] : container) {
			os << key << ':' << value;
			if (++number != size) {
				os << ',';
			}
		}
		os << ']';
		return os;
	}

	template <typename Tkey, typename Tvalue>
	std::ostream& operator << (std::ostream& os, const std::unordered_multimap<Tkey, Tvalue>& container)
	{
		const size_t size = container.size();
		size_t number = 0;
		os << '[';
		for (auto&& [key, value] : container) {
			os << key << ':' << value;
			if (++number != size) {
				os << ',';
			}
		}
		os << ']';
		return os;
	}

	template <typename T>
	std::ostream& operator << (std::ostream& os, const std::shared_ptr<T>& sPtr)
	{
		if (!sPtr) {
			os << "null";
			return os;
		}
		os << *sPtr;
		return os;
	};

	template <typename T>
	std::ostream& operator << (std::ostream& os, const std::weak_ptr<T>& wPtr)
	{
		if (wPtr.expired()) {
			os << "null";
			return os;
		}
		std::shared_ptr<T> sPtr = wPtr.lock();
		if (!sPtr) {
			os << "null";
			return os;
		}
		os << *sPtr;
		return os;
	};

	template <typename T>
	std::ostream& operator << (std::ostream& os, const std::unique_ptr<T>& uPtr)
	{
		if (!uPtr) {
			os << "null";
			return os;
		}
		os << *uPtr;
		return os;
	};
}
