//Nick Sells, 2023

#ifndef CAMERA_H
#define CAMERA_H

#include <iostream>

#include "level.h"
#include "video.h"

class Camera {

private:
	bool dirty;

public:
	Vector3 position;
	float yaw;
	
	float fov;
	float aspect;
	float near;
	float far;

	Matrix4 V;
	Matrix4 P;

	//typical member-wise constructor
	inline Camera(Vector3 position, float yaw, float fov, float aspect, float near, float far):
	position(position), yaw(yaw), fov(fov), aspect(aspect), near(near), far(far) {
		CalculateP();
		CalculateV();
	}

	//updates the view matrix of this camera
	inline void CalculateV(void) noexcept {
		V = Matrix4::RotationY(-yaw) * Matrix4::Translation(-position);
	}
	
	inline void CalculateP(void) {
		float focalLength = 1/tan(fov/2);
		float denom = 1/(far-near);
		P[0][0] = focalLength/aspect;
		P[1][1] = focalLength;
		P[2][2] = (near+far)*denom;
		P[2][3] = (2*near*far)*denom;
		P[3][2] = -1;
	}

	inline void Render(const Model& model) noexcept {

		CalculateP();
		CalculateV();

		Matrix4 PV = P*V;

		for(const auto& vertex : model.verts) {
			Vector4 clip = PV*vertex.AsPoint();
			
			//don't worry about any vertices outside of the cube of (-1,-1,-1) to (1,1,1) 
			// if((1.0f >= abs(clip[0])) || (1.0f >= abs(clip[1])) || (1.0f >= abs(clip[2])))
			// 	continue;
			//^^^ for some reason this check causes nothing to be drawn
			
			Vector3 ndc = clip / clip[3];
			Video::PlotPixel(ndc[0], ndc[1]);
		}
	}

	inline void Render(const GameObject& gobj) noexcept {

		CalculateP();
		CalculateV();

		Matrix4 PVM = P*V*gobj.transform;

		for(const auto& vertex : gobj.mesh.verts) {
			Vector4 clip = PVM*vertex.AsPoint();
			
			//don't worry about any vertices outside of the cube of (-1,-1,-1) to (1,1,1) 
			// if((1.0f >= abs(clip[0])) || (1.0f >= abs(clip[1])) || (1.0f >= abs(clip[2])))
			// 	continue;
			//^^^ for some reason this check causes nothing to be drawn
			
			Vector3 ndc = clip / clip[3];
			Video::PlotPixel(ndc[0], ndc[1]);
		}
	}
};

#endif
