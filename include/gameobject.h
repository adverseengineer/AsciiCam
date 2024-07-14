//Nick Sells, 2023-2024

#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <memory>
#include "model.h"
#include "util.h"

//base class for all objects in a scene
class GameObject {

public:
	glm::mat4 transform;
	const Model& mesh;

	inline GameObject(const glm::mat4& transform, const Model& mesh):
	transform(transform), mesh(mesh) {
	}

	inline GameObject(const glm::vec3& position, const Model& mesh):
	transform(glm::mat4(1.0f)), mesh(mesh) {
		transform[3] = glm::vec4(position, 1.0f); 
	}

	//appends a game object text representation to an output stream
	inline friend std::ostream& operator<<(std::ostream& stream, const GameObject& object) {
		stream << "Transform:\n";
		Util::appendToStream<4,4>(stream, object.transform);
		stream << "Mesh:\n" << object.mesh << '\n';		
		return stream;
	}
};

#endif
