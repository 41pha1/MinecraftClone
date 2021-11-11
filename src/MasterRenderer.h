#ifndef MASTERRENDERER_H_
#define MASTERRENDERER_H_

#include "Game.h"

class MasterRenderer {
public:
	glm::vec3 SKY_COLOR = glm::vec3(0.7f, 0.7f, 0.9f);
	MasterRenderer();
	MasterRenderer(int width, int height);
	virtual ~MasterRenderer();
	void render(Game* game);
	Camera * camera;
};

#endif
