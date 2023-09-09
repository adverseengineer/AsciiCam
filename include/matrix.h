//Nick Sells, 2023

#ifndef MATRIX_H
#define MATRIX_H

#include "vector.h"

//a matrix with N rows and N columns
template <size_t Rows, size_t Cols>
class Matrix {
private:
	float elems[Rows][Cols];
	constexpr static float epsilon = 1e-6;

public:
	//default constructor
	inline constexpr Matrix(void) noexcept {
		for(size_t row = 0; row < Rows; row++)
			for(size_t col = 0; col < Cols; col++)
				this->elems[row][col] = 0.0f;
	}

	//element-wise constructor
	template <typename... Args>
    inline constexpr Matrix(const Args&... args) noexcept {
		static_assert(sizeof...(args) == Rows * Cols, "incorrect number of elements supplied");
		const float values[] = {static_cast<float>(args)...};
		for(size_t row = 0; row < Rows; row++)
			for(size_t col = 0; col < Cols; col++)
				this->elems[row][col] = values[row * Cols + col];
	}

	//typical copy constructor
	inline constexpr Matrix(const Matrix& other) noexcept {
		for(size_t row = 0; row < Rows; row++)
			for(size_t col = 0; col < Cols; col++)
				this->elems[row][col] = other.elems[row][col];
	}

	//used as a container for the pointer returned from the matrix [] operator
	//allows us to do bounds checking on the 2nd dimension index 
	struct RowProxy {
    private:
        float* rowData;

    public:
		//constructs a new row proxy with the given pointer
        inline constexpr RowProxy(float* data) noexcept : rowData(data) {}

		//allows access to the elements pointed to by this row proxy
        inline float& operator[](size_t col) noexcept {
            assert(col < Cols);
			assert(rowData != nullptr);
			return rowData[col];
        }

		//writes the contents of an initializer list to the row pointed to by this proxy
		//TODO: figure out how to rewrite using variadic templates bc initializer lists
		//disqualify this method from being constexpr-able and i hate them
		inline RowProxy& operator=(std::initializer_list<float> args) noexcept {
			assert(args.size() == Cols);
			assert(rowData != nullptr);
			size_t col = 0;
			for(auto elem : args)
				this->rowData[col++] = elem;
			return (*this);
		}
    };

	//returns a RowProxy object of the row-th row of this matrix, which can then be indexed for the elements
	inline constexpr RowProxy operator[](size_t row) {
		assert(row < Rows);
		return RowProxy(this->elems[row]);
	}

	//multiplies this matrix by another
	//rows*cols x otherRows*otherCols = rows*otherCols
	//otherRows = cols
	//NOTE: will not compile if the matrices are incompatible for multiplication
	template<size_t OtherCols>
	inline constexpr Matrix<Rows,OtherCols> operator*(const Matrix<Cols,OtherCols>& other) const noexcept {
		Matrix<Rows,OtherCols> result;
		for(size_t row = 0; row < Rows; row++) {
			for(size_t otherCol = 0; otherCol < OtherCols; otherCol++) {
				for(size_t otherRow = 0; otherRow < Cols; otherRow++) {
					result.elems[row][otherCol] += this->elems[row][otherRow] * other.elems[otherRow][otherCol];
					//if the calculated value is basically zero, make it really zero
					if((-epsilon <= result.elems[row][otherCol]) && (result.elems[row][otherCol] <= epsilon))
						result.elems[row][otherCol] = 0.0f;
				}
			}
		}
		return result;
	}

	//multiplies this matrix by a column vector
	inline constexpr Vector<Rows> operator*(const Vector<Rows>& vec) const noexcept {
		//NOTE: this is just a special case of matrix multiplication. we treat the vector as an Nx1 matrix
		Vector<Rows> result;
		for(size_t row = 0; row < Rows; row++) {
			for(size_t col = 0; col < Cols; col++)
				result.elems[row] += this->elems[row][col] * vec.elems[col];
	
			if((-epsilon <= result.elems[row]) && (result.elems[row] <= epsilon))
				result.elems[row] = 0.0f;
		}
		return result;
	}

	//multiplies each element of this matrix by a scalar
	inline constexpr Matrix operator*(float scalar) const noexcept {
		Matrix result;
		for(size_t row = 0; row < Rows; row++)
			for(size_t col = 0; col < Cols; col++)
				result.elems[row][col] = this->elems[row][col] * scalar;
		return result;
	}

	//divides each element of this matrix by a scalar
	inline constexpr Matrix operator/(float scalar) const noexcept {
		assert(scalar != 0.0f); //cannot divide by zero
		Matrix result;
		for(size_t row = 0; row < Rows; row++)
			for(size_t col = 0; col < Cols; col++)
				result.elems[row][col] = this->elems[row][col] / scalar;
		return result;
	}

	//checks if two matrices are equal
	inline constexpr bool operator==(const Matrix& other) {
		for(size_t row = 0; row < Rows; row++)
			for(size_t col = 0; col < Cols; col++)
				if(this->elems[row][col] != other.elem[row][col])
					return false;
		return true;
	}

	//appends a matrix to an output stream
	inline friend std::ostream& operator<<(std::ostream& stream, const Matrix& matrix) {

		const int precision = 15;
		const int chars = 19;

		for(size_t row = 0; row < Rows; row++) {
			stream << "[ ";
			for(size_t col = 0; col < Cols; col++)
				stream << std::fixed << std::setprecision(precision) << std::setw(chars) << matrix.elems[row][col] << " ";
			stream << "]\n";
		}

		return stream;
	}

	//creates an NxN identity matrix
	inline static constexpr Matrix Identity(void) noexcept {
		static_assert(Rows == Cols, "identity is defined only for square matrices");
		Matrix<Rows,Rows> result;
		for(size_t i = 0; i < Rows; i++)
			result.elems[i][i] = 1.0f;
		return result;
	}

	//creates a 3D translation matrix
	inline static Matrix<4,4> Translation(const Vector<3>& delta) {	
		auto result = Matrix<4,4>::Identity();
		for(size_t i = 0; i < 3; i++)
			result.elems[i][3] = delta.elems[i];
		return result;
	}

	//creates a matrix that rotates theta radians about the origin in the YZ-plane
	inline static Matrix<4,4> RotationX(float pitch) noexcept {
		auto result = Matrix<4,4>::Identity();
		float c = cos(pitch);
		float s = sin(pitch);
		result.elems[1][1] = result.elems[2][2] = c;
		result.elems[1][2] = -s;
		result.elems[2][1] = s;
		return result;
	}

	//creates a matrix that rotates theta radians about the origin in the XZ-plane
	inline static Matrix<4,4> RotationY(float yaw) noexcept {
		auto result = Matrix<4,4>::Identity();
		float c = cos(yaw);
		float s = sin(yaw);
		result.elems[0][0] = result.elems[2][2] = c;
		result.elems[0][2] = s;
		result.elems[2][0] = -s;
		return result;
	}

	//creates a matrix that rotates theta radians about the origin in the XY-plane
	inline static Matrix<4,4> RotationZ(float roll) noexcept {
		auto result = Matrix<4,4>::Identity();
		float c = cos(roll);
		float s = sin(roll);
		result[0][0] = result[1][1] = c;
		result[0][1] = -s;
		result[1][0] = s;
		return result;
	}

	//creates a matrix that combines X, Y, and Z axis rotation using euler angles
	//TODO: verify everything is correct, i got this formula system where the z-axis is up
	inline static Matrix<4,4> Rotation(float pitch, float yaw, float roll) noexcept {
		auto result = Matrix<4,4>::Identity();
		float cx = cos(pitch);
		float sx = sin(pitch);
		float cy = cos(yaw);
		float sy = sin(yaw);
		float cz = cos(roll);
		float sz = sin(roll);
		result[0][0] = cy*cz;
		result[0][1] = sx*sy*cz-cx*sz;
		result[0][2] = cx*sy*cz+sx*sz;
		result[1][0] = cy*sz;
		result[1][1] = sx*sy*sz+cx*cz;
		result[1][2] = cx*sy*sz-sx*cz;
		result[2][0] = -sy;
		result[2][1] = sx*cy;
		result[2][2] = cx*cy;
		return result;
	}

	//uses Rodrigues' rotation formula to create an angle-axis rotation matrix
	inline static Matrix<4,4> Rotation(const Vector<3>& axis, float theta) {
		auto result = Matrix<4,4>::Identity();
		
		float c = cos(theta);
		float s = sin(theta);
		float oneMinusC = 1-c;

		result[0][0] = oneMinusC * axis[0] * axis[0] + c;
		result[0][1] = oneMinusC * axis[0] * axis[1] - axis[2] * s;
		result[0][2] = oneMinusC * axis[0] * axis[2] + axis[1] * s;
		result[1][0] = oneMinusC * axis[0] * axis[1] + axis[2] * s;
		result[1][1] = oneMinusC * axis[1] * axis[1] + c;
		result[1][2] = oneMinusC * axis[1] * axis[2] - axis[0] * s;
		result[2][0] = oneMinusC * axis[0] * axis[2] - axis[1] * s;
		result[2][1] = oneMinusC * axis[1] * axis[2] + axis[0] * s;
		result[2][2] = oneMinusC * axis[2] * axis[2] + c;

		return result;
	}
};

typedef Matrix<3,3> Matrix3;
typedef Matrix<4,4> Matrix4;

#endif
