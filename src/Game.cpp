#include "Game.h"

#include <glm/detail/type_vec3.hpp>
#include <windef.h>
#include <winnt.h>
#include <climits>
#include <cstdlib>
#include <iostream>
#include <utility>

#include "Block.h"
#include "Camera.h"
#include "Player.h"
#include "WorldGenerator.h"

Game::Game(Camera *  cam)
{
	cam->game = this;
	generator = new WorldGenerator(1234L);
	player = new Player(this);
	chunks = std::map<int, std::map<int, std::map<int, Chunk *>>>();

	std::cout << "generating spawn area..." << std::endl;

	int px = player->pos.x  / Chunk::SIZE;
	int pz = player->pos.z  / Chunk::SIZE;
	for(int x = -RENDER_DISTANCE; x < RENDER_DISTANCE; x++)
	{
		for(int z = -RENDER_DISTANCE; z < RENDER_DISTANCE; z++)
		{
			for(int y = 0; y < 3; y++)
			{
				chunks[x+px][y][z+pz] = loadChunk(x+px, y, z+pz);
			}
		}
	}

	std::cout << "meshing generated chunks..." << std::endl;
	for(auto x : chunks)
	{
		for(auto y : x.second)
		{
			for(auto z : y.second)
			{
				z.second->generateMesh();
				renderQueue.push_back(z.second);
			}
		}
	}
	player->setOnGround();
	camera = cam;
	chunkProvider = new ChunkProvider();
}

Chunk* Game::loadChunk(int cx, int cy, int cz)
{
	Chunk * chunk = new Chunk(this, 0, cx, cy ,cz);
	return chunk;
}

char Game::getBlock(int x, int y, int z)
{
	int cx = (x + (x<0 ? 1 : 0)) / Chunk::SIZE - (x<0 ? 1 : 0);
	int cy = (y + (y<0 ? 1 : 0)) / Chunk::SIZE - (y<0 ? 1 : 0);
	int cz = (z + (z<0 ? 1 : 0)) / Chunk::SIZE - (z<0 ? 1 : 0);

	Chunk * chunk = getChunk(cx, cy, cz);

	if(!chunk->valid)
		return invalidBlock;

	glm::vec3 chunkCoords = toChunkCoords(x, y, z);
	return chunk->blocks[(int)chunkCoords.x][(int)chunkCoords.y][(int)chunkCoords.z];
}

bool Game::isChunkLoaded(int cx, int cy, int cz)
{
	if(chunks.count(cx) == 0)
		return false;
	if(chunks[cx].count(cy) == 0)
		return false;

	if (chunks[cx][cy].count(cz) == 0)
		return false;

	return chunks[cx][cy][cz] != NULL;
}

void Game::updateRenderQueue()
{
	renderQueue.clear();
	int x = player->pos.x;
	int y = player->pos.y;
	int z = player->pos.z;

	int pcx = (x + (x<0 ? 1 : 0)) / Chunk::SIZE - (x<0 ? 1 : 0);
	int pcy = (y + (y<0 ? 1 : 0)) / Chunk::SIZE - (y<0 ? 1 : 0);
	int pcz = (z + (z<0 ? 1 : 0)) / Chunk::SIZE - (z<0 ? 1 : 0);

	int RENDER_DISTANCE_SQUARED = RENDER_DISTANCE * RENDER_DISTANCE;
	for(int dx = - RENDER_DISTANCE; dx <= RENDER_DISTANCE; dx++)
	{
		int dx_squared = dx * dx;
		for(int dy = - RENDER_DISTANCE; dy <= RENDER_DISTANCE; dy++)
		{
			if(dy*dy > RENDER_DISTANCE_SQUARED)
				continue;

			for(int dz = - RENDER_DISTANCE; dz <= RENDER_DISTANCE; dz++)
			{
				if((dx_squared + dz*dz)>RENDER_DISTANCE_SQUARED )
					continue;

				int cx = pcx + dx;
				int cy = pcy + dy;
				int cz = pcz + dz;

				if(isChunkLoaded(cx, cy, cz))
				{
					renderQueue.push_back(getChunk(cx, cy, cz));
				}
			}
		}
	}
}

DWORD WINAPI threadFunction(LPVOID lpParameter)
{
	Game * game = (Game*) lpParameter;
	game->loadChunks();
	std::cout << "done" << std::endl;
    return 0;
}


void Game::loadChunks()
{
	int x = player->pos.x;
	int z = player->pos.z;

	int pcx = (x + (x<0 ? 1 : 0)) / Chunk::SIZE - (x<0 ? 1 : 0);
	int pcz = (z + (z<0 ? 1 : 0)) / Chunk::SIZE - (z<0 ? 1 : 0);

	std::map<int, std::map<int, std::map<int, bool>>> chunksToRemesh;

	int RENDER_DISTANCE_SQUARED = RENDER_DISTANCE * RENDER_DISTANCE;


	int closest = INT_MAX;
	int ccx, ccz;

	for(int dx = - RENDER_DISTANCE; dx <= RENDER_DISTANCE; dx++)
	{
		int dx_squared = dx * dx;

		for(int dz =  -RENDER_DISTANCE; dz <= RENDER_DISTANCE; dz++)
		{
			if((dx_squared + dz*dz)>RENDER_DISTANCE_SQUARED)
				continue;

			int distance = std::abs(dx) + std::abs(dz);

			if(distance >= closest)
				continue;


			int cx = pcx + dx;
			int cz = pcz + dz;

			if(!isChunkLoaded(cx, 0, cz))
			{
				closest = distance;
				ccx = cx;
				ccz = cz;
			}
		}
	}

	if(closest == INT_MAX)
		return;

	for(int y = 0; y <= 3; y++)
	{
		chunkProvider->requestChunk(ccx, y, ccz);
		chunks[ccx][y][ccz] = loadChunk(ccx, y, ccz);

		chunksToRemesh[ccx][y][ccz] = true;
		chunksToRemesh[ccx-1][y][ccz] = true;
		chunksToRemesh[ccx+1][y][ccz] = true;
		chunksToRemesh[ccx][y-1][ccz] = true;
		chunksToRemesh[ccx][y+1][ccz] = true;
		chunksToRemesh[ccx][y][ccz+1] = true;
		chunksToRemesh[ccx][y][ccz-1] = true;
	}

	for(auto x : chunksToRemesh)
	{
		int cx = x.first;
		for(auto y : x.second)
		{
			int cy = y.first;
			for(auto z : y.second)
			{
				int cz = z.first;
				if(z.second &&  isChunkLoaded(cx, cy, cz))
				{
					Chunk * chunk = getChunk(cx, cy, cz);
					chunk->generateMesh();
				}
			}
		}
	}
}

Chunk* Game::getChunk(int cx, int cy, int cz)
{
	if(isChunkLoaded(cx, cy, cz))
		return chunks[cx][cy][cz];

	return invalidChunk;
}

void Game::update(float dt)
{
	camera->update(player);
	player->move();
	chunkProvider->update();

//    HANDLE hThread = CreateThread(NULL,0,threadFunction,this,0,NULL);
//    WaitForSingleObject(hThread, INFINITE);
//    CloseHandle(hThread);

	loadChunks();
	updateRenderQueue();
}

Game::~Game()
{
	delete invalidChunk;
}

void Game::tick()
{
	gametick++;
	player->updateVelocity();
	for(glm::vec3 blockPos : updateQueue)
	{
		Block::update(getBlock(blockPos.x, blockPos.y, blockPos.z), this, blockPos.x, blockPos.y, blockPos.z);
	}
	updateQueue.clear();
}

glm::vec3 Game::toChunkCoords(int x, int y, int z)
{
	int bx = x < 0 ?  (((x) % Chunk::SIZE + Chunk::SIZE) % Chunk::SIZE): x % Chunk::SIZE;
	int by = y < 0 ?  (((y) % Chunk::SIZE + Chunk::SIZE) % Chunk::SIZE): y % Chunk::SIZE;
	int bz = z < 0 ?  (((z) % Chunk::SIZE + Chunk::SIZE) % Chunk::SIZE): z % Chunk::SIZE;
	return glm::vec3(bx, by, bz);
}

void Game::setBlock(int x, int y, int z, int id)
{
	int cx = (x + (x<0 ? 1 : 0)) / Chunk::SIZE - (x<0 ? 1 : 0);
	int cy = (y + (y<0 ? 1 : 0)) / Chunk::SIZE - (y<0 ? 1 : 0);
	int cz = (z + (z<0 ? 1 : 0)) / Chunk::SIZE - (z<0 ? 1 : 0);

	Chunk * chunk = getChunk(cx, cy, cz);

	if(!chunk->valid)
		return;

	glm::vec3 chunkCoords = toChunkCoords(x, y, z);

	chunk->blocks[(int)chunkCoords.x][(int)chunkCoords.y][(int)chunkCoords.z] = id;
}

void Game::placeBlock(int x, int y, int z, int id)
{
	setBlock(x, y, z, id);
	int cx = (x + (x<0 ? 1 : 0)) / Chunk::SIZE - (x<0 ? 1 : 0);
	int cy = (y + (y<0 ? 1 : 0)) / Chunk::SIZE - (y<0 ? 1 : 0);
	int cz = (z + (z<0 ? 1 : 0)) / Chunk::SIZE - (z<0 ? 1 : 0);
	getChunk(cx, cy, cz)->generateMesh();

	updateQueue.push_back(glm::vec3(x+1, y, z));
	updateQueue.push_back(glm::vec3(x-1, y, z));
	updateQueue.push_back(glm::vec3(x, y+1, z));
	updateQueue.push_back(glm::vec3(x, y-1, z));
	updateQueue.push_back(glm::vec3(x, y, z+1));
	updateQueue.push_back(glm::vec3(x, y, z-1));

	glm::vec3 chunkCoords = toChunkCoords(x, y, z);

	if(chunkCoords.x == 0)
		getChunk(cx-1, cy, cz)->generateMesh();
	if(chunkCoords.x== Chunk::SIZE-1)
		getChunk(cx+1, cy, cz)->generateMesh();
	if(chunkCoords.y == 0)
		getChunk(cx, cy-1, cz)->generateMesh();
	if(chunkCoords.y == Chunk::SIZE-1)
		getChunk(cx, cy+1, cz)->generateMesh();
	if(chunkCoords.z== 0)
		getChunk(cx, cy, cz-1)->generateMesh();
	if(chunkCoords.z == Chunk::SIZE-1)
		getChunk(cx, cy, cz+1)->generateMesh();
}
