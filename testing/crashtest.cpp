//Nick Sells, 2023
//main.cpp

#include <iostream>
#include "matrix.h"

int main(int argc, char** argv) {

	Matrix<3,2> b = {
		3,-5,
		5,-3,
		2,3
	};
	Matrix<2,3> c = {
		-5,3,2,
		-5,-5,-5
	};
	auto bc = b*c;
	std::cout << bc;
}
