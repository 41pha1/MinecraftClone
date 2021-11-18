#include "Camera.h"

#include <glm/ext/matrix_transform.hpp>
#include <glm/geometric.hpp>
#include <glm/trigonometric.hpp>
#include "glm/glm.hpp"
#include "glm/ext.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <cmath>

#include "Block.h"
#include "Game.h"
#include "Player.h"

Camera::Camera(float FOV, int width, int height, float NEAR_, float FAR_)
{
	pos = glm::vec3(10,10,10);
	yaw = 0;
	pitch = 0;
	roll = 0;
	View = glm::lookAt(pos, glm::vec3(0,0,0), glm::vec3(0,1,0));
	Projection = glm::perspective(glm::radians(FOV),  width / (float) height, NEAR_, FAR_);
	blockInReach = false;
}

float mod(float value, int modulus)
{
  return std::fmod((std::fmod(value, modulus) + modulus), modulus);
}

float intbound(float s, float ds)
{
  // Find the smallest positive t such that s+t*ds is an integer.
  if (ds < 0) {
    return intbound(-s, -ds);
  } else {
    s = mod(s, 1);
    // problem is now s+t*ds = 1
    return (1-s)/ds;
  }
}

template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}

void Camera::calculateLookingAt(glm::vec3 dir)
{
    float range = 5.0f; //max range to check (in voxels)
    float xPos = floor(pos.x);
    float yPos = floor(pos.y);
    float zPos = floor(pos.z);
    int stepX = sgn(dir.x);
    int stepY = sgn(dir.y);
    int stepZ = sgn(dir.z);
    glm::vec3 tMax(intbound(pos.x, dir.x), intbound(pos.y, dir.y), intbound(pos.z, dir.z));
    glm::vec3 tDelta((float)stepX / dir.x, (float)stepY / dir.y, (float)stepZ / dir.z);
    float faceX;
    float faceY;
    float faceZ;

    blockInReach = false;

	do {
		if (Block::hasBlockCollision(game->getBlock(xPos, yPos, zPos)))
		{
			blockInReach = true;
			lookingAt.x = xPos;
			lookingAt.y = yPos;
			lookingAt.z = zPos;

			placeBlock.x = xPos + faceX;
			placeBlock.y = yPos + faceY;
			placeBlock.z = zPos + faceZ;
			break;
		}
		if (tMax.x < tMax.y) {
			if (tMax.x < tMax.z) {
				if (tMax.x > range) break;

				xPos += stepX;
				tMax.x += tDelta.x;

				faceX = -stepX;
				faceY = 0;
				faceZ = 0;
			} else {
				if (tMax.z > range) break;
				zPos += stepZ;
				tMax.z += tDelta.z;
				faceX = 0;
				faceY = 0;
				faceZ = -stepZ;
			}
		} else {
			if (tMax.y < tMax.z) {
				if (tMax.y > range) break;
				yPos += stepY;
				tMax.y += tDelta.y;
				faceX = 0;
				faceY = -stepY;
				faceZ = 0;
			} else {
				if (tMax.z > range) break;
				zPos += stepZ;
				tMax.z += tDelta.z;
				faceX = 0;
				faceY = 0;
				faceZ = -stepZ;
			}
		}
	} while (true);
}


Camera::~Camera()
{

}

void Camera::update(Player * player)
{
	pos = player->pos + glm::vec3(0., 0.72, 0.);
	yaw = player->yaw;
	pitch = player->pitch;

	glm::vec3 direction;
	direction.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
	direction.y = sin(glm::radians(pitch));
	direction.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
	direction = glm::normalize(direction);

	calculateLookingAt(direction);

	View = glm::lookAt(pos, pos + direction, glm::vec3(0, 1, 0));
}
