//Nick Sells

#ifndef VECTOR_H
#define VECTOR_H

#include <array>
#include <cassert>
#include <cmath>
#include <cstring>
#include <concepts>
#include <iomanip>
#include <ostream>

#include "util.h"

//an N-dimensional vector
template <size_t N>
struct Vector {

	float elems[N];

	//default constructor
	inline constexpr Vector(void) noexcept {
		for(size_t i = 0; i < N; i++)
			this->elems[i] = 0.0f;
	}

	//element-wise constructor
	template <typename... Args>
    inline constexpr Vector(const Args... args) {
		static_assert(sizeof...(args) == N, "incorrect number of elements supplied");
		const float values[] = {static_cast<float>(args)...};
		for(size_t i = 0; i < N; i++)
			this->elems[i] = values[i];
	}

	//typical copy constructor
	inline constexpr Vector(const Vector& other) noexcept {
		for(size_t i = 0; i < N; i++)
			this->elems[i] = other.elems[i];
	}

	//an implicit conversion constructor for promoting or demoting vectors to higher or lower dimensions
	template <size_t M>
    inline constexpr Vector(const Vector<M>& other) {
		size_t numElems = std::min(N,M);
		//copy only as many elements as will fit
		for(size_t i = 0; i < numElems; i++)
			this->elems[i] = other.elems[i];
		for(size_t i = numElems; i < N; i++)
			this->elems[i] = 0.0f;
    }

	//allows access to a vector's components via indexing
	inline float& operator[](size_t elem) noexcept {
		// static_assert(elem < N, "index out of range");
		assert(elem < N); //index out of range
		return this->elems[elem];
	}

	//allows access to a vector's components via indexing
	//NOTE: this overload is necessary to access elements of vectors passed as const references
	inline float operator[](size_t elem) const noexcept {
		// static_assert(elem < N, "index out of range");
		assert(elem < N); //index out of range
		return this->elems[elem];
	}

	//adds each component of this vector and another
	inline constexpr Vector operator+(const Vector& other) const noexcept {
		Vector result;
		for(size_t i = 0; i < N; i++)
			result.elems[i] = this->elems[i] + other.elems[i];
		return result;
	}

	//subtracts each component another vector from this one
	inline constexpr Vector operator-(const Vector& other) const noexcept {
		Vector result;
		for(size_t i = 0; i < N; i++)
			result.elems[i] = this->elems[i] - other.elems[i];
		return result;
	}

	//negates all components of a vector
	inline constexpr Vector operator-(void) const noexcept {
		Vector result;
		for(size_t i = 0; i < N; i++)
			result.elems[i] = -this->elems[i];
		return result;
	}

	//multiplies each component of this vector by a scalar value
	inline constexpr Vector operator*(float scalar) const noexcept {
		Vector result;
		for(size_t i = 0; i < N; i++)
			result.elems[i] = this->elems[i] * scalar;
		return result;
	}

	//divides each component of this vector by a scalar value
	inline constexpr Vector operator/(float scalar) const noexcept {
		// if(scalar == 0.0f)
		// 	throw std::runtime_error("cannot divide by zero");
		assert(scalar != 0.0f); //cannot divide by zero
		Vector result;
		for(size_t i = 0; i < N; i++)
			result.elems[i] = this->elems[i] / scalar;
		return result;
	}

	//checks if two vectors are equal
	inline constexpr bool operator==(const Vector& other) const noexcept {
		for(size_t i = 0; i < N; i++)
			if(this->elems[i] != other.elems[i])
				return false;
		return true;
	}

	//appends a vector text representation to an output stream
	inline friend std::ostream& operator<<(std::ostream& stream, const Vector& vector) noexcept {
		
		const int precision = 15;
		const int chars = 19;
		
		stream << "( ";
		for (size_t i = 0; i < N; i++)
			stream << std::fixed << std::setprecision(precision) << std::setw(chars) << vector.elems[i] << " ";
		stream << ')';
		return stream;
	}

	//creates a 4D vector from this vector and sets W to 1
	inline constexpr Vector<4> AsPoint(void) const noexcept {
		static_assert(N == 3, "this method is defined only for vectors of three dimensions");
		Vector<4> result = (*this);
		result.elems[3] = 1;
		return result;
	}

	//calculates the cross product of this vector and another
	inline constexpr Vector<3> Cross(const Vector<3>& other) const noexcept {
		
		static_assert(N == 3, "cross product is defined only for vectors of three dimensions");

		Vector result;
		result.elems[0] = this->elems[1]*other.elems[2] - this->elems[2]*other.elems[1];
		result.elems[1] = this->elems[2]*other.elems[0] - this->elems[0]*other.elems[2];
		result.elems[2] = this->elems[0]*other.elems[1] - this->elems[1]*other.elems[0];
		return result;
	}

	//calculates the dot product of this vector and another
	inline constexpr float Dot(const Vector& other) const noexcept {
		float result = 0.0f;
		for(size_t i = 0; i < N; i++)
			result += this->elems[i] * other.elems[i];
		return result;
	}

	//calculates the length of this vector
	inline constexpr float Magnitude(void) const noexcept {
		return sqrt(this->Dot(*this));
	}

	//calculates the distance between this vector and another
	inline constexpr float Distance(const Vector& other = Vector()) const noexcept {
		Vector diff = (*this) - other;
		return diff.Magnitude();
	}

	//returns a vector that points in the same direction but has a magnitude of one
	inline constexpr Vector Normalize(void) const noexcept {
		return (*this) / this->Magnitude();
	}

	//pre-defined unit vectors
	static const Vector UP;
	static const Vector DOWN;
	static const Vector LEFT;
	static const Vector RIGHT;
	static const Vector FORWARD;
	static const Vector BACKWARD;
};

typedef Vector<2> Vector2;
typedef Vector<3> Vector3;
typedef Vector<4> Vector4;

template <>
constexpr Vector2 Vector2::UP = {0.0f, 1.0f};
template<>
constexpr Vector2 Vector2::DOWN = {0.0f, -1.0f};
template <>
constexpr Vector2 Vector2::LEFT = {-1.0f, 0.0f};
template <>
constexpr Vector2 Vector2::RIGHT = {1.0f, 0.0f};
template <>
constexpr Vector3 Vector3::UP = {0.0f, 1.0f, 0.0f};
template <>
constexpr Vector3 Vector3::DOWN = {0.0f, -1.0f, 0.0f};
template <>
constexpr Vector3 Vector3::LEFT = {-1.0f, 0.0f, 0.0f};
template <>
constexpr Vector3 Vector3::RIGHT = {1.0f, 0.0f, 0.0f};
template <>
constexpr Vector3 Vector3::FORWARD = {0.0f, 0.0f, 1.0f};
template <>
constexpr Vector3 Vector3::BACKWARD = {0.0f, 0.0f, -1.0f};

#endif
