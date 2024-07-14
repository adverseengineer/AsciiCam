
#ifndef UTIL_H
#define UTIL_H

#include <cmath>
#include <limits>

#include <glm/matrix.hpp>
#include <ostream>
#include <iomanip>

namespace Util {

	static const int precision = 15;
	static const int chars = 19;

	//converts a matrix into a stream representation
	template <size_t C, size_t R, typename T = float>
	static inline void appendToStream(std::ostream& stream, const glm::mat<C,R,T>& mat) {
		for(size_t row = 0; row < R; row++) {
			stream << "[ ";
			for(size_t col = 0; col < C; col++)
				stream << std::fixed << std::setprecision(precision) << std::setw(chars) << mat[row][col] << " ";
			stream << "]\n";
		}
	}
	
	//converts a vector into a stream representation
	template <size_t L, typename T = float>
	static inline void appendToStream(std::ostream& stream, const glm::vec<L,T> vec) {
		stream << "( ";
		for(size_t i = 0; i < L; i++)
			stream << std::fixed << std::setprecision(precision) << std::setw(chars) << vec[i] << " ";
		stream << ")\n";
	}
}
#endif
