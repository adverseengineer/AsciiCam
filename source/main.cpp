//Nick Sells, 2023
//main.cpp

#include <csignal>
#include "camera.h"

#define DEGTORAD ((M_PI)/180.0f)
#define RADTODEG (180.0f/(M_PI))

size_t frameCounter = 0;
int lastInput = ERR;

void UpdateInfoBlob(Camera& cam, GameObject& gobj) {
	const size_t infoBufferLen = 128;
	char infoBuffer[infoBufferLen];
	int line = 0;

	snprintf(infoBuffer, infoBufferLen, "frame #%lu", frameCounter++);
	mvaddnstr(line++, 0, infoBuffer, infoBufferLen);

	snprintf(infoBuffer, infoBufferLen, "last input: %d", lastInput);
	mvaddnstr(line++, 0, infoBuffer, infoBufferLen);

	snprintf(infoBuffer, infoBufferLen, "cam pos: (%f, %f, %f)", cam.position[0], cam.position[1], cam.position[2]);
	mvaddnstr(line++, 0, infoBuffer, infoBufferLen);

	snprintf(infoBuffer, infoBufferLen, "cam yaw: %f", cam.yaw*RADTODEG);
	mvaddnstr(line++, 0, infoBuffer, infoBufferLen);

	snprintf(infoBuffer, infoBufferLen, "cam fov: %f", cam.fov*RADTODEG);
	mvaddnstr(line++, 0, infoBuffer, infoBufferLen);

 	snprintf(infoBuffer, infoBufferLen, "cam aspect: %f", cam.aspect);
	mvaddnstr(line++, 0, infoBuffer, infoBufferLen);

	line = Video::ScreenHeight - 1;

	snprintf(infoBuffer, infoBufferLen, "[ %f %f %f %f ]", gobj.transform[3][0], gobj.transform[3][1], gobj.transform[3][2], gobj.transform[3][3]);
	mvaddnstr(line--, 0, infoBuffer, infoBufferLen);
	snprintf(infoBuffer, infoBufferLen, "[ %f %f %f %f ]", gobj.transform[2][0], gobj.transform[2][1], gobj.transform[2][2], gobj.transform[2][3]);
	mvaddnstr(line--, 0, infoBuffer, infoBufferLen);
	snprintf(infoBuffer, infoBufferLen, "[ %f %f %f %f ]", gobj.transform[1][0], gobj.transform[1][1], gobj.transform[1][2], gobj.transform[1][3]);
	mvaddnstr(line--, 0, infoBuffer, infoBufferLen);
	snprintf(infoBuffer, infoBufferLen, "[ %f %f %f %f ]", gobj.transform[0][0], gobj.transform[0][1], gobj.transform[0][2], gobj.transform[0][3]);
	mvaddnstr(line--, 0, infoBuffer, infoBufferLen);
	snprintf(infoBuffer, infoBufferLen, "model matrix:");
	mvaddnstr(line--, 0, infoBuffer, infoBufferLen);

	snprintf(infoBuffer, infoBufferLen, "[ %f %f %f %f ]", cam.V[3][0], cam.V[3][1], cam.V[3][2], cam.V[3][3]);
	mvaddnstr(line--, 0, infoBuffer, infoBufferLen);
	snprintf(infoBuffer, infoBufferLen, "[ %f %f %f %f ]", cam.V[2][0], cam.V[2][1], cam.V[2][2], cam.V[2][3]);
	mvaddnstr(line--, 0, infoBuffer, infoBufferLen);
	snprintf(infoBuffer, infoBufferLen, "[ %f %f %f %f ]", cam.V[1][0], cam.V[1][1], cam.V[1][2], cam.V[1][3]);
	mvaddnstr(line--, 0, infoBuffer, infoBufferLen);
	snprintf(infoBuffer, infoBufferLen, "[ %f %f %f %f ]", cam.V[0][0], cam.V[0][1], cam.V[0][2], cam.V[0][3]);
	mvaddnstr(line--, 0, infoBuffer, infoBufferLen);
	snprintf(infoBuffer, infoBufferLen, "view matrix:");
	mvaddnstr(line--, 0, infoBuffer, infoBufferLen);

	snprintf(infoBuffer, infoBufferLen, "[ %f %f %f %f ]", cam.P[3][0], cam.P[3][1], cam.P[3][2], cam.P[3][3]);
	mvaddnstr(line--, 0, infoBuffer, infoBufferLen);
	snprintf(infoBuffer, infoBufferLen, "[ %f %f %f %f ]", cam.P[2][0], cam.P[2][1], cam.P[2][2], cam.P[2][3]);
	mvaddnstr(line--, 0, infoBuffer, infoBufferLen);
	snprintf(infoBuffer, infoBufferLen, "[ %f %f %f %f ]", cam.P[1][0], cam.P[1][1], cam.P[1][2], cam.P[1][3]);
	mvaddnstr(line--, 0, infoBuffer, infoBufferLen);
	snprintf(infoBuffer, infoBufferLen, "[ %f %f %f %f ]", cam.P[0][0], cam.P[0][1], cam.P[0][2], cam.P[0][3]);
	mvaddnstr(line--, 0, infoBuffer, infoBufferLen);
	snprintf(infoBuffer, infoBufferLen, "projection matrix:");
	mvaddnstr(line--, 0, infoBuffer, infoBufferLen);
}

void GetInput(Camera& cam, Vector3& forward, Vector3& right) {
	lastInput = getch();
	switch(lastInput) {
		case ERR: break;
		case 'w': cam.position = cam.position + forward; break;
		case 'a': cam.position = cam.position - right; break;
		case 's': cam.position = cam.position - forward; break;
		case 'd': cam.position = cam.position + right; break;
		case 'q': cam.position[1] -= 0.5f; break;
		case 'e': cam.position[1] += 0.5f; break;
		case 'z': cam.fov += 10*DEGTORAD; break;
		case 'x': cam.fov -= 10*DEGTORAD; break;
		case KEY_LEFT: cam.yaw += 10*DEGTORAD; break;
		case KEY_RIGHT: cam.yaw -= 10*DEGTORAD; break;
	}
}

int main(void) {

	Video::Init();

	Camera cam({0,0,-5}, 90*DEGTORAD, 60*DEGTORAD, 1, 0.1, 10);
	Vector3 forward;
	Vector3 right;

	Model mod(
		{{1,1,1},{1,1,-1},{1,-1,1},{1,-1,-1},{-1,1,1},{-1,1,-1},{-1,-1,1},{-1,-1,-1}},
		{{1,7,5},{1,3,7},{1,4,3},{1,2,4},{3,8,7},{3,4,8},{5,7,8},{5,8,6},{1,5,6},{1,6,2},{2,6,8},{2,8,4}}
	);

	GameObject gobj(Matrix4::Identity(), mod);
	float animYaw = 0;

	while(true) {
		Video::Clear();

		animYaw += 1*DEGTORAD;
		gobj.transform = Matrix4::Translation({0, cos(15*animYaw), 0}) * Matrix4::RotationY(7*animYaw);
		
		UpdateInfoBlob(cam, gobj);
		cam.Render(gobj);

		Video::Refresh();

		forward[0] = sin(cam.yaw) * 0.5f;
		forward[2] = cos(cam.yaw) * 0.5f;
		right[0] = -forward[2];
		right[2] = forward[0];

		GetInput(cam, forward, right);
	}

	Video::Deinit();
}
