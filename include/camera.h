//Nick Sells, 2023

#ifndef CAMERA_H
#define CAMERA_H

#include <iostream>

#include "level.h"
#include "video.h"

#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>

class Camera {

public:
	glm::mat4 transform;
	float fov;
	float near;
	float far;

	glm::mat4 view;
	glm::mat4 projection;

	inline void UpdateView() {
		view = glm::inverse(transform);
	}

	inline void UpdatePerspective() {
		projection = glm::perspective(glm::radians(fov), Video::GetAspectRatio(), near, far);
	}

	//typical member-wise constructor
	inline Camera(const glm::mat4& transform, float fov, float near, float far):
	transform(transform), fov(fov), near(near), far(far) {
		UpdateView();
		UpdatePerspective();
	}

	inline Camera (const glm::vec3& position, float fov, float near, float far):
	transform(glm::mat4(1.0f)), fov(fov), near(near), far(far) {
		transform[3] = glm::vec4(position, 1.0f); 
	}

	inline void Render(const GameObject& gobj) {

		UpdateView();
		UpdatePerspective();

		//pre-calculate the combined transformation matrix
		glm::mat4 PVM = (projection * view) * gobj.transform;
		
		std::size_t numVerts = gobj.mesh.verts.size();
		glm::vec4* clipVerts = new glm::vec4[numVerts];
		glm::vec3* ndcVerts = new glm::vec3[numVerts];
		glm::vec3* screenVerts = new glm::vec3[numVerts]; //vec3 bc we need the depth too
		bool* visible = new bool[numVerts];

		//send each vertex through the pipeline
		for(size_t i = 0; i < numVerts; i++) {
			//convert to clip space
			clipVerts[i] = PVM * glm::vec4(gobj.mesh.verts[i], 1.0f);
			//convert to normalized device coordinates
			ndcVerts[i] = clipVerts[i] / clipVerts[i].w;
			//convert to screen space coordinates
			screenVerts[i].x = (ndcVerts[i].x + 1.0f) * 0.5f * Video::GetScreenWidth();
			screenVerts[i].y = (1.0f - ndcVerts[i].y) * 0.5f * Video::GetScreenHeight();				
			//the vertex is only visible if it lies within the unit cube of -1.0 to 1.0
			visible[i] = (abs(ndcVerts[i].x) < 1.0f) && (abs(ndcVerts[i].y) < 1.0f) && (abs(ndcVerts[i].z) < 1.0f);
		}

		size_t numIndices = gobj.mesh.indices.size();
		switch (gobj.mesh.renderingPrimitive) {
			case Model::Primitive::Points:
				for (size_t i = 0; i < numIndices; i++) {
					const auto& v0 = screenVerts[gobj.mesh.indices[i]];
					Video::PlotPixel(v0.x, v0.y);
				}
				break;
			case Model::Primitive::Lines:
				if (numIndices % 2 != 0)
					throw std::runtime_error("number of indices is not a multiple of two required for line rendering");
				for (size_t i = 0; i < numIndices; i += 2) {
					const auto& v0 = screenVerts[gobj.mesh.indices[i]];
					const auto& v1 = screenVerts[gobj.mesh.indices[i+1]];
					Video::PlotLine(v0.x, v0.y, v1.x, v1.y);
				}
				break;
			case Model::Primitive::Triangles:
				if (numIndices % 2 != 0)
					throw std::runtime_error("number of indices is not a multiple of three required for triangle rendering");
				for (size_t i = 0; i < numIndices; i += 3) {
					const auto& v0 = screenVerts[gobj.mesh.indices[i]];
					const auto& v1 = screenVerts[gobj.mesh.indices[i+1]];
					const auto& v2 = screenVerts[gobj.mesh.indices[i+2]];
					Video::PlotLine(v0.x, v0.y, v1.x, v1.y);
					Video::PlotLine(v1.x, v1.y, v2.x, v2.y);
					Video::PlotLine(v2.x, v2.y, v0.x, v0.y);
				}
				break;
			default:
				throw std::runtime_error("unknown rendering primitive");
		}

		delete[] clipVerts;
		delete[] ndcVerts;
		delete[] screenVerts;
		delete[] visible;
	}
};

#endif
