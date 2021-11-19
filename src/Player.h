#ifndef PLAYER_H_
#define PLAYER_H_

#include <glm/detail/type_vec3.hpp>
#include <glm/ext/vector_float3.hpp>

class Game;

class Player {
public:
	float SPEED = 0.18f;
	float SLIPPERINESS = 0.6f;     //0.6 default, 0.8 slime, 0.98 ice, 1.0 airborne
	float DRAG = 0.98f;            //++
	float GRAVITY = 0.1f;
	float JUMP_POWER = 0.55f;      //++
	float FLYING_SPEED = 2.f;
	bool FLYING = true;
	bool COLLISION = true;

	bool onGround = false;
	glm::vec3 hitrad  = glm::vec3(0.3, 0.9, 0.3);
	glm::vec3 acc = glm::vec3(0,0,0);
	glm::vec3 pos =  glm::vec3(-215-1000, 71, 275);
	glm::vec3 vel = glm::vec3(0,0,0);
	float pitch = 0.5f, yaw = 0.5f;
	Game * game;

	void move(float dt);
	void updateVelocity();
	void accelerate(glm::vec3 acc);
	void setOnGround();
	bool isColliding();
	Player(Game * game);
	virtual ~Player();
};

#endif /* PLAYER_H_ */
