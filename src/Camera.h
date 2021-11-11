#ifndef CAMERA_H_
#define CAMERA_H_

#include <glm/detail/type_vec3.hpp>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/vector_float3.hpp>

class Game;

class Player;

class Camera
{
public:
	bool renderWireframe = false;
	glm::vec3 pos= glm::vec3(0,0,0);
	glm::mat4 Projection;
	glm::mat4 View;
	glm::vec3 lookingAt;
	glm::vec3 placeBlock;
	bool blockInReach;
	double yaw = 0, pitch = 0, roll = 0;
	Game * game = 0;

	Camera(float, int, int, float, float);

	void calculateLookingAt(glm::vec3 dir);

	void update(Player * player);

	virtual ~Camera();
};

#endif
