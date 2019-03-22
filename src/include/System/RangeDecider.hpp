#ifndef SYSTEM_RANGEDECIDER_HPP
#define SYSTEM_RANGEDECIDER_HPP

#include <vector>
#include <tuple>

template <typename T>
class RangeDecider {
	
	T fallback;
	std::vector<T> elements;
	std::vector<std::tuple<float, float>> ranges;
	
	bool inRange(float p, size_t i) {
		auto &r = ranges.at(i);
		return (p >= std::get<0>(r) && p <= std::get<1>(r));
	}
public:
	
	RangeDecider(T fallbackElement):
		fallback(fallbackElement)
	{
	}
	
	void addRange(T elem, float min, float max) {
		elements.push_back(elem);
		ranges.push_back(std::make_tuple(min, max));
	}
	
	T at(float p) {
		for (size_t i = 0; i < ranges.size(); ++i) {
			if (inRange(p, i)) return elements.at(i);
		}
		
		return fallback;
	}
	
	void clear() {
		elements.clear();
		ranges.clear();
	}
};

#endif
