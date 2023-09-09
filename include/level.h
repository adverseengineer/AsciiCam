//Nick Sells, 2023

#ifndef LEVEL_H
#define LEVEL_H

#include "camera.h"
#include "gameobject.h"

class Level {

public:

	std::vector<GameObject> objects;

	Level(void);

	Level(const std::string& path) {
		assert(!"not implemented");	
	}

	Level(const std::vector<GameObject>& objects):
		objects(objects) {
	}
};

#endif
