// ◦ Xyz ◦
#pragma once

#include <atomic>
#include <array>
#include <Singletone.h>

class DebugContext : public mystd::Singletone<DebugContext>
{
public:
	class ValueGraph
	{
	public:
		ValueGraph() {
			_values.resize(_maxSize, 0.f);
		}

		ValueGraph(size_t maxSize, short int resetMax)
			: _resetMax(resetMax)
			, _maxSize(maxSize){
			_values.resize(_maxSize, 0.f);
		}

		ValueGraph(ValueGraph&& valueGraph) noexcept
		{
			std::swap(_resetMax, valueGraph._resetMax);
			std::swap(_maxValue, valueGraph._maxValue);
			std::swap(_maxSize, valueGraph._maxSize);
			std::swap(_values, valueGraph._values);
		}

		void operator = (ValueGraph&& valueGraph) noexcept
		{
			std::swap(_resetMax, valueGraph._resetMax);
			std::swap(_maxValue, valueGraph._maxValue);
			std::swap(_maxSize, valueGraph._maxSize);
			std::swap(_values, valueGraph._values);
		}

		int Count() const {
			return static_cast<int>(_values.size());
		}

		const float* Data() const {
			return _values.data();
		}
	
		float MaxValue() const {
			return _maxValue;
		}

		void Add(float value) {
			_values.erase(_values.begin());
			_values.emplace_back(value);

			if (_resetMax-- == 0) {
				_resetMax = 10;
				_maxValue = *std::max_element(_values.begin(), _values.end());

			}
			else {
				_maxValue = std::max(value, _maxValue);
			}
		}
	
	private:
		short int _resetMax = 10;
		float _maxValue = 0.f;
		size_t _maxSize = 100;
		std::vector<float> _values;

	};

	DebugContext() = default;

	int countObject = 0;
	float diameterSpace = 0.f;
	float minForce = 0.f;
	float maxForce = 0.f;
	float minVelocity = 0.f;
	float maxVelocity = 0.f;
	float minMass = 0.f;
	float maxMass = 0.f;
	float sumMass = 0.f;

	float constForce = 1.f;
	float constSpeed = 1.f;

	std::vector<double> deltaTimes;
	float deltaTime = 0.f;
	float subProgress = 0.f;
	float progress = 0.f;
	ValueGraph fpsGraph;
	ValueGraph cpsGraph;
	ValueGraph countObjectGraph = ValueGraph(100, -1);

	void Clean() {
		countObject = 0.f;
		diameterSpace = 0.f;
		minForce = std::numeric_limits<float>::max();
		maxForce = std::numeric_limits<float>::min();
		minVelocity = std::numeric_limits<float>::max();
		maxVelocity = std::numeric_limits<float>::min();
		minMass = std::numeric_limits<float>::max();
		maxMass = std::numeric_limits<float>::min();
		sumMass = 0.f;

		subProgress = 0.f;
		progress = 0.f;
	}
};
