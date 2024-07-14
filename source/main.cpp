//Nick Sells, 2023
//main.cpp

#include <csignal>
#include "camera.h"
#include <glm/ext/matrix_transform.hpp>

extern "C" {
#include <ncurses.h>
}
//TODO: remove this include and instead get input from a dedicated input system that hides ncurses from code that doesn't need to see it
//TODO: also migrate UI functionality into its own system which uses ncurses too

unsigned long frameCounter = 0;
int lastInput = ERR;

void ShowMatrix(const char* msg, char* infoBuffer, size_t len, int& line, glm::mat4& mat) {
	snprintf(infoBuffer, len, msg, ' ');
	mvaddnstr(line++, 0, infoBuffer, len);
	for(size_t i = 0; i < 4; i++) {
		snprintf(infoBuffer, len, "[ %.3f %.3f %.3f %.3f ]", mat[0][i], mat[1][i], mat[2][i], mat[3][i]);
		mvaddnstr(line++, 0, infoBuffer, len);
	}
}

void UpdateInfoBlob(Camera& cam, GameObject& gobj) {
	const size_t infoBufferLen = 128;
	char infoBuffer[infoBufferLen];
	int line = 0;

	snprintf(infoBuffer, infoBufferLen, "frame #%lu", frameCounter++);
	mvaddnstr(line++, 0, infoBuffer, infoBufferLen);

	snprintf(infoBuffer, infoBufferLen, "last input: %d", lastInput);
	mvaddnstr(line++, 0, infoBuffer, infoBufferLen);

	snprintf(infoBuffer, infoBufferLen, "WASD: move laterally");
	mvaddnstr(line++, 0, infoBuffer, infoBufferLen);

	snprintf(infoBuffer, infoBufferLen, "Q/E: move up/down");
	mvaddnstr(line++, 0, infoBuffer, infoBufferLen);

	snprintf(infoBuffer, infoBufferLen, "left/right: turn");
	mvaddnstr(line++, 0, infoBuffer, infoBufferLen);

	snprintf(infoBuffer, infoBufferLen, "Z/X: increase/decrease FOV");
	mvaddnstr(line++, 0, infoBuffer, infoBufferLen);

	snprintf(infoBuffer, infoBufferLen, "screen dimensions: %ux%u", Video::GetScreenWidth(), Video::GetScreenHeight());
	mvaddnstr(line++, 0, infoBuffer, infoBufferLen);

	snprintf(infoBuffer, infoBufferLen, "aspect ratio: %.3f", Video::GetAspectRatio());
	mvaddnstr(line++, 0, infoBuffer, infoBufferLen);

	snprintf(infoBuffer, infoBufferLen, "cam fov: %.3f", cam.fov);
	mvaddnstr(line++, 0, infoBuffer, infoBufferLen);

	ShowMatrix("camera transform:", infoBuffer, infoBufferLen, line, cam.transform);
	ShowMatrix("camera view:", infoBuffer, infoBufferLen, line, cam.view);
	ShowMatrix("camera projection:", infoBuffer, infoBufferLen, line, cam.projection);
	ShowMatrix("obj transform:", infoBuffer, infoBufferLen, line, gobj.transform);
}

void UseInput(Camera& cam) {
	switch(lastInput) {
		case ERR: break;
		case 'w': cam.transform[3] += cam.transform[2]; break;
		case 'a': cam.transform[3] += cam.transform[0]; break;
		case 's': cam.transform[3] -= cam.transform[2]; break;
		case 'd': cam.transform[3] -= cam.transform[0]; break;
		case 'q': cam.transform[3].y -= 0.5f; break;
		case 'e': cam.transform[3].y += 0.5f; break;
		case 'z': cam.fov -= 5.0f; break;
		case 'x': cam.fov += 5.0f; break;
		case KEY_LEFT:
			cam.transform = glm::rotate(cam.transform, -glm::radians(10.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			break;
		case KEY_RIGHT:
			cam.transform = glm::rotate(cam.transform, glm::radians(10.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			break;
	}
}

int main(void) {

	Video::Init();

	Camera cam(glm::vec3(0.0f, 0.0f, 5.0f), 60.0f, 0.1f, 10.0f);
	
	Model cube(
		{{1,1,1},{1,1,-1},{1,-1,1},{1,-1,-1},{-1,1,1},{-1,1,-1},{-1,-1,1},{-1,-1,-1}},
		{0,1, 1,3, 3,2, 2,0, 4,5, 5,7, 7,6, 6,4, 0,4, 1,5, 2,6, 3,7},
		Model::Primitive::Lines
	);

	float sqrt2 = sqrt(2);
	Model tetrahedron(
		{{1,0,1},{1,0,-1},{-1,0,-1},{-1,0,1},{0,sqrt2,0},{0,-sqrt2,0}},
		{0,1, 1,2, 2,3, 3,0, 0,4, 1,4, 2,4, 3,4, 0,5, 1,5, 2,5, 3,5},
		Model::Primitive::Lines
	);

	GameObject gobj1(glm::vec3(-3.0f, 0.0f, 0.0f), cube);
	GameObject gobj2(glm::vec3(3.0f, 0.0f, 0.0f), tetrahedron);
	float anim = 0;

	while(true) {

		Video::Clear();

		Video::PlotLine(30.0f, 0.0f, 30.0f, 32.0f, 0);

		anim += glm::radians(1.0f);

		gobj1.transform = glm::translate(
			glm::rotate(
				glm::mat4(1.0f),
				3*anim,
				glm::vec3(0.0f, 1.0f, 0.0f)
			),
			glm::vec3(
				3.0f,
				sin(5*anim),
				0.0f
			)
		);

		gobj2.transform = glm::translate(
			glm::rotate(
				glm::mat4(1.0f),
				4.5f*anim,
				glm::vec3(1.0f)
			),
			glm::vec3(
				3.0f,
				sin(0.2*anim),
				0.0f
			)
		);

		UpdateInfoBlob(cam, gobj1);
		cam.Render(gobj1);
		cam.Render(gobj2);
		Video::Refresh();

		lastInput = getch();
		UseInput(cam);
	}

	Video::Deinit();
}
