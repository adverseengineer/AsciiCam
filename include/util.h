
#ifndef UTIL_H
#define UTIL_H

#include <cmath>
#include <limits>

namespace Util {
	template <typename T>
	static inline bool ApproxEqual(T a, T b, T epsilon = std::numeric_limits<T>::epsilon()) noexcept {

		T diff = std::abs(a - b);

		//if the operands are truly equal
		if(a == b)
			return true;
		//if the elements are 
		else if(a == 0 || b == 0 || diff < std::numeric_limits<T>::min())
			// Handle values near zero and subnormal numbers
			return diff < (epsilon * std::numeric_limits<T>::min());
		else {
			T absA = std::abs(a);
			T absB = std::abs(b);
			return diff / std::min(absA + absB, std::numeric_limits<T>::max()) < epsilon;
		}
	}
}
#endif
