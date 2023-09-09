//Nick Sells, 2023

#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <memory>
#include "matrix.h"
#include "model.h"

//base class for all objects in a scene
class GameObject {

public:
	Matrix4 transform;
	const Model& mesh;

	GameObject(const Matrix4& transform, const Model& mesh):
		transform(transform), mesh(mesh) {
	}

	//appends a game object text representation to an output stream
	inline friend std::ostream& operator<<(std::ostream& stream, const GameObject& object) {
		stream << "Transform:\n" << object.transform << "Mesh:\n" << object.mesh << '\n';		
		return stream;
	}
};

#endif
