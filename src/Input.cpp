#include "Input.h"

#include <glm/detail/type_vec3.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/trigonometric.hpp>
#include <SDL_keycode.h>
#include <cmath>

#include "Block.h"
#include "Camera.h"
#include "Game.h"
#include "Mouse.h"
#include "Player.h"

Input::Input()
{

}

void Input::update(std::map<int, bool> & keyboard, Mouse * mouse, Game * game)
{
	if(keyboard[SDLK_0])
	{
		game->camera->renderWireframe = !game->camera->renderWireframe;
		keyboard[SDLK_0] = false;
	}

	float sin = std::sin(glm::radians(game->player->yaw));
	float cos = std::cos(glm::radians(game->player->yaw));

	glm::vec3 movement = glm::vec3(0,0,0);
	if(keyboard[SDLK_s])
		movement += glm::vec3(-1, 0, 0) * cos + glm::vec3(0, 0, -1) * sin;

	if(keyboard[SDLK_w])
		movement += glm::vec3(1, 0, 0) * cos + glm::vec3(0, 0, 1) * sin;

	if(keyboard[SDLK_d])
		movement += glm::vec3(0, 0, 1) * cos + glm::vec3(-1, 0, 0) * sin;

	if(keyboard[SDLK_a])
		movement += glm::vec3(0, 0, -1) * cos + glm::vec3(1, 0, 0) * sin;

//	if(keyboard[SDLK_SPACE])
//		movement += glm::vec3(0, 1, 0);
//
//	if(keyboard[SDLK_LSHIFT])
//		movement += glm::vec3(0, -1, 0);

	if(keyboard[SDLK_SPACE])
	{
		if(game->player->FLYING)
			movement.y += 1;
		else if(game->player->onGround)
			game->player->vel.y += game->player->JUMP_POWER;
	}

	if(keyboard[SDLK_LSHIFT])
	{
		if(game->player->FLYING)
			movement.y -= 1;
		else
			movement *= 0.3;
	}

	else if(keyboard[SDLK_LCTRL])
	{
		if(game->player->FLYING)
			movement *= 4.f;
		else
			movement *= 1.15;
		if(!game->player->onGround && !game->player->FLYING)
			movement += glm::vec3(0.2, 0, 0) * cos + glm::vec3(0, 0, 0.2) * sin;
	}

	if(mouse->left && game->camera->blockInReach)
	{
		mouse->left = false;
		glm::vec3 lookingAt = game->camera->lookingAt;
	    game->placeBlock(lookingAt.x, lookingAt.y, lookingAt.z, Block::AIR);
	}
	if(mouse->right && game->camera->blockInReach)
	{
		mouse->right = false;
		glm::vec3 placeBlock = game->camera->placeBlock;
		game->placeBlock(placeBlock.x, placeBlock.y, placeBlock.z, Block::GLASS);
	}



	game->player->accelerate(movement * game->player->SPEED);

	game->player->pitch -= (mouse->dy) * 1.;
	game->player->yaw += (mouse->dx) * 1.;

	mouse->dy = 0;
	mouse->dx = 0;

	if(game->player->pitch > 89)
		game->player->pitch = 89;

	if(game->player->pitch < -89)
		game->player->pitch = -89;
}

Input::~Input()
{

}

