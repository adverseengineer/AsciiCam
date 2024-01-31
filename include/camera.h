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

		// for(const auto& face : gobj.mesh.faces) {

		// 	Vector4 clip0 = PVM*gobj.mesh.verts[face.vertexIndices[0]].AsPoint();
		// 	Vector4 clip1 = PVM*gobj.mesh.verts[face.vertexIndices[1]].AsPoint();
		// 	Vector4 clip2 = PVM*gobj.mesh.verts[face.vertexIndices[2]].AsPoint();
		// 	// Vector4 clip3 = PVM*gobj.mesh.verts[3].AsPoint();

		// 	Vector3 ndc0 = clip0/clip0[3];
		// 	Vector3 ndc1 = clip1/clip1[3];
		// 	Vector3 ndc2 = clip2/clip2[3];
		// 	// Vector3 ndc3 = clip3/clip3[3];

		// 	int x0 = (int)(Video::ScreenWidth * (ndc0[0] + 1)/2 + 0.5f);
		// 	int y0 = (int)(Video::ScreenHeight * (1 - ndc0[1])/2 + 0.5f);
		// 	int x1 = (int)(Video::ScreenWidth * (ndc1[0] + 1)/2 + 0.5f);
		// 	int y1 = (int)(Video::ScreenHeight * (1 - ndc1[1])/2 + 0.5f);
		// 	int x2 = (int)(Video::ScreenWidth * (ndc2[0] + 1)/2 + 0.5f);
		// 	int y2 = (int)(Video::ScreenHeight * (1 - ndc2[1])/2 + 0.5f);
		// 	// int x3 = (int)(Video::ScreenWidth * (ndc3[0] + 1)/2 + 0.5f);
		// 	// int y3 = (int)(Video::ScreenHeight * (1 - ndc3[1])/2 + 0.5f);

		// 	// Video::PlotLine(x0,y0,x1,y1);
		// 	// Video::PlotLine(x0,y0,x2,y2);
		// 	// Video::PlotLine(x0,y0,x3,y3);
		// 	Video::PlotPixel(x0,y0);
		// 	Video::PlotPixel(x1,y1);
		// 	Video::PlotPixel(x2,y2);
		// }

		for(const auto& vertex : gobj.mesh.verts) {
			Vector4 clip = PVM*vertex.AsPoint();
			
			Vector3 ndc = clip / clip[3];

			int x = (int)(Video::ScreenWidth * (ndc[0] + 1)/2 + 0.5f);
			int y = (int)(Video::ScreenHeight * (1 - ndc[1])/2 + 0.5f);

			Video::PlotPixel(x, y);
		}
	}

	// inline void Render(const GameObject& gobj) noexcept {

	// 	CalculateP();
	// 	CalculateV();

	// 	Matrix4 PVM = P*V*gobj.transform;

	// 	for(const auto& vertex : gobj.mesh.verts) {
	// 		Vector4 clip = PVM*vertex.AsPoint();
			
	// 		//don't worry about any vertices outside of the cube of (-1,-1,-1) to (1,1,1) 
	// 		// if((1.0f >= abs(clip[0])) || (1.0f >= abs(clip[1])) || (1.0f >= abs(clip[2])))
	// 		// 	continue;
	// 		//^^^ for some reason this check causes nothing to be drawn
			
	// 		Vector3 ndc = clip / clip[3];

	// 		int x = (int)(Video::ScreenWidth * (ndc[0] + 1)/2 + 0.5f);
	// 		int y = (int)(Video::ScreenHeight * (1 - ndc[1])/2 + 0.5f);

	// 		Video::PlotPixel(x, y);
	// 	}
	// }
};

#endif
