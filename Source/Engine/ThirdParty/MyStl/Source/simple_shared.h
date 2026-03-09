#pragma once

#include <exception>

namespace mystd
{
	template <typename T>
	class simple_shared_ptr final {
	public:
		simple_shared_ptr() = default;

		simple_shared_ptr(const simple_shared_ptr& ptrRef)
			: _dataPtr(ptrRef._dataPtr)
			, _counter(ptrRef._counter)
		{
			if (!_counter) {
				simple_shared_ptr<T>* ptr = const_cast<simple_shared_ptr<T>*>(&ptrRef);
				ptr->_counter = new size_t(1);
				_counter = ptr->_counter;
			}
			IncreaseCounter();
		}

		simple_shared_ptr(simple_shared_ptr&& ptr) noexcept {
			std::swap(_dataPtr, ptr._dataPtr);
			std::swap(_counter, ptr._counter);
		}

		simple_shared_ptr(T* dataPtr)
			: _dataPtr(dataPtr)
			, _counter(nullptr)
		{}
		
		~simple_shared_ptr() {
			DecreaseCounter();
		}

		simple_shared_ptr& operator = (const simple_shared_ptr& ptrRef) {
			DecreaseCounter();
			_dataPtr = nullptr;
			_counter = nullptr;

			if (!ptrRef._counter) {
				simple_shared_ptr<T>* ptr = const_cast<simple_shared_ptr<T>*>(&ptrRef);
				ptr->_counter = new size_t(1);
			}
			_counter = ptrRef._counter;

			if (IncreaseCounter()) {
				_dataPtr = ptrRef._dataPtr;
			}
			return *this;
		}

		simple_shared_ptr& operator = (simple_shared_ptr&& ptr) noexcept {
			std::swap(_dataPtr, ptr._dataPtr);
			std::swap(_counter, ptr._counter);
			return *this;
		}

		T* get() {
			return _dataPtr;
		}

		T* get() const {
			return _dataPtr;
		}

		T& operator * () {
			return *_dataPtr;
		}
		
		T& operator * () const {
			return *_dataPtr;
		}
		
		T* operator -> () {
			return _dataPtr;
		}

		T* operator -> () const {
			return _dataPtr;
		}

		operator bool() const {
			return _dataPtr;
		}

		bool operator == (const simple_shared_ptr& ptr) const {
			return _dataPtr == ptr._dataPtr;
		}

		void Reset() {
			DecreaseCounter();
		}
	
	private:
		bool IncreaseCounter() {
			if (!_counter) {
				return false;
			}

			++(*_counter);
			return true;
		}

		void DecreaseCounter() {
			if (_counter) {
				--(*_counter);

				if (*_counter == 0) {
					delete _dataPtr;
					delete _counter;
				}
			}
			else {
				delete _dataPtr;
			}
		}

	public:
		template <typename ...Args>
		static simple_shared_ptr make(Args&&... args) {
			T* dataPtr = new T(std::forward<Args>(args)...);
			return simple_shared_ptr(dataPtr);
		}

	private:
		T* _dataPtr = nullptr;
		size_t * _counter = nullptr;
	};

	template <typename T>
	using shared_ptr = simple_shared_ptr<T>;

	template <typename T, typename... Args>
	simple_shared_ptr<T> make_shared(Args&&... args)
	{
		return simple_shared_ptr<T>::make(std::forward<Args>(args)...);
	}
}
