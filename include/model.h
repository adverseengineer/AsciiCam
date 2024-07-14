//Nick Sells, 2023

#ifndef MODEL_H
#define MODEL_H

#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include <glm/vec3.hpp>

#include "util.h"

struct Model {

	enum class Primitive : unsigned char {
		Points = 0,
		Lines = 1,
		Triangles = 2,
		//Quads = 3 //TODO: add support for quads, but it will necessitate extra checks to make sure verts are coplanar
	};

	std::vector<glm::vec3> verts;
	std::vector<unsigned short> indices;
	Primitive renderingPrimitive;

	inline Model(const std::vector<glm::vec3>& verts, const std::vector<unsigned short>& indices, Primitive renderingPrimitive):
	verts(verts), indices(indices), renderingPrimitive(renderingPrimitive) {
	}

	//appends a text representation of a models verts and indices to an output stream
	inline friend std::ostream& operator<<(std::ostream& stream, const Model& model) {
		
		stream << "verts: {";
		size_t n = model.verts.size();
		for (size_t i = 0; i < n; i++) {
			Util::appendToStream<3>(stream, model.verts[i]);
			if(i < n - 1)
				stream << ", ";
		}
		stream << "}\nindices: {";
		n = model.indices.size();
		for (size_t i = 0; i < n; i++) {
			stream << model.indices[i];
			if(i < n - 1)
				stream << ", ";
		}
		stream << '}';
		
		return stream;
	}
};

#endif
