#include "Player.h"

#include <cmath>
#include <glm/ext/matrix_transform.hpp>
#include <glm/geometric.hpp>
#include <glm/trigonometric.hpp>
#include "glm/glm.hpp"
#include "glm/ext.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Block.h"
#include "Game.h"

class Game;

Player::Player(Game * game_)
{
	game = game_;
}


bool Player::isColliding()
{
	if(!COLLISION)
		return false;
	for(int dx = -2; dx <= 2; dx++)
	{
		for(int dy = -2; dy <= 2; dy++)
		{
			for(int dz = -2; dz <= 2; dz++)
			{
				int bx = (int)(pos.x + dx);
				int by = (int)(pos.y + dy);
				int bz = (int)(pos.z + dz);
				char block = game->getBlock(bx, by, bz);
				if(Block::hasBlockCollision(block))
				{
					if((pos.x-hitrad.x <= bx + 1 && pos.x + hitrad.x >= bx) &&
					   (pos.y-hitrad.y <= by + 1 && pos.y + hitrad.y >= by) &&
					   (pos.z-hitrad.z <= bz + 1 && pos.z + hitrad.z >= bz))
						return true;
				}
			}
		}
	}
	return false;
}

void Player::setOnGround()
{
	while(!isColliding())
		pos.y--;
	while(isColliding())
		pos.y++;
}

void Player::accelerate(glm::vec3 acc_)
{
	acc += acc_;
}

void Player::move(float dt)
{
	onGround = false;

	glm::vec3 frameVel = dt * vel/3.f;


	if(isColliding())
	{
		pos.x += frameVel.x;
		pos.z += frameVel.z;
	}else
	{
		pos += frameVel;

		if(isColliding())
		{
			pos -= frameVel;

			pos.x += frameVel.x;
			if(isColliding())
			{
				pos.x -= frameVel.x;
				vel.x *= 0.f;
			}

			pos.y += frameVel.y;
			if(isColliding())
			{
				pos.y -= frameVel.y;
				vel.y *= 0.f;
				onGround = true;
			}

			pos.z += frameVel.z;
			if(isColliding())
			{
				pos.z -= frameVel.z;
				vel.z *= 0.f;
			}
		}
	}
}

void Player::updateVelocity()
{
	if(!FLYING)
	{
		vel.y -= GRAVITY;
		vel.y *= DRAG;
	}else
	{
		vel.y = vel.y * SLIPPERINESS * 0.91f;
	}


	vel.x = vel.x * SLIPPERINESS * 0.91f;
	vel.z = vel.z * SLIPPERINESS * 0.91f;

	vel += (FLYING ? FLYING_SPEED : SPEED) * acc * std::pow((0.6f/SLIPPERINESS),3.f);

	acc = glm::vec3(0,0,0);
}

Player::~Player()
{

}
