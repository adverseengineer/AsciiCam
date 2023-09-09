//Nick Sells, 2023

#ifndef MODEL_H
#define MODEL_H

#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "vector.h"

struct Model {

	struct Face {
		size_t vertexIndices[3];

		Face(const std::initializer_list<size_t>& indices) {
			assert(indices.size() == 3);
			size_t i = 0;
			for(const size_t& elem : indices)
				vertexIndices[i++] = elem;
		}

		//appends a face's text representation to an output stream
		inline friend std::ostream& operator<<(std::ostream& stream, const Face& face) {
			stream << '[';
			for (size_t i = 0; i < 3; i++) {
				stream << face.vertexIndices[i];
				if(i < 2)
					stream << ", ";
			}
			stream << ']';
			return stream;
		}
	};

	std::vector<Vector3> verts;
	std::vector<Face> faces;

	Model(const std::vector<Vector3>& verts, const std::vector<Face>& faces):
		verts(verts), faces(faces) {
	}

	//appends a face text representation to an output stream
	inline friend std::ostream& operator<<(std::ostream& stream, const Model& model) {
		stream << "verts: {";
		size_t n = model.verts.size();
		for (size_t i = 0; i < n; i++) {
			stream << model.verts[i];
			if(i < n - 1)
				stream << ", ";
		}
		stream << "}\nfaces: {";
		n = model.faces.size();
		for (size_t i = 0; i < n; i++) {
			stream << model.faces[i];
			if(i < n - 1)
				stream << ", ";
		}
		stream << '}';
		
		return stream;
	}
};

#endif
