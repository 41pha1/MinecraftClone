#include "Game.h"

#include <glm/detail/type_vec3.hpp>
#include <winbase.h>
#include <windef.h>
#include <winnt.h>
#include <array>
#include <climits>
#include <cstdlib>
#include <iostream>
#include <utility>

#include "Block.h"
#include "Player.h"

Game::Game(Camera *  cam, std::string worldFolder_)
{
	cam->game = this;
	generator = new WorldGenerator(1234L);
	player = new Player(this);
	chunks = std::map<int, std::map<int, std::map<int, Chunk *>>>();
	wLoader = new MinecraftWorldLoader(this);

	if(worldFolder_ != "")
	{
		worldFolder = worldFolder_;
		loadWorld = true;
	}

	std::cout << "preparing spawn area..." << std::endl;

	int px = player->pos.x  / Chunk::SIZE;
	int pz = player->pos.z  / Chunk::SIZE;
	for(int x = -1; x < 1; x++)
	{
		for(int z = -1; z < 1; z++)
		{
			auto chunk = loadChunk(x+px, z+pz);
			for(int y = 0; y < Chunk::HEIGHT; y++)
			{
				chunks.operator[](x+px)[y][z+pz] = (*chunk)[y];
			}
		}
	}

	std::cout << "meshing chunks..." << std::endl;
	for(auto x : chunks)
	{
		for(auto y : x.second)
		{
			for(auto z : y.second)
			{
				if(z.second->blocks != 0)
				{
					z.second->generateMesh();
					z.second->loadToVao();
					renderQueue.push_back(z.second);
				}
			}
		}
	}
	std::cout << "Done!" << std::endl;
	player->setOnGround();
	camera = cam;
	chunkProvider = new ChunkProvider(this);
}

std::array<Chunk *, Chunk::HEIGHT> *  Game::loadChunk(int cx, int cz)
{
	if(loadWorld)
	{
//		std::cout << "Loading chunk " << cx << ", " << cz << " from disk." << std::endl;
		return wLoader->loadChunk(cx, cz, worldFolder);
	}
	auto * chunk = new std::array<Chunk *, Chunk::HEIGHT>;
	for(int i = 0; i < Chunk::HEIGHT; i++)
	{
		(*chunk)[i] =  new Chunk(this, 0, cx, i ,cz, true);
	}
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
	return chunk->getBlock((char)chunkCoords.x, (char)chunkCoords.y, (char)chunkCoords.z);
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

				if(isChunkLoaded(cx, cy, cz) && getChunk(cx, cy, cz)->blocks != 0)
				{
					renderQueue.push_back(getChunk(cx, cy, cz));
				}
			}
		}
	}
}

void Game::loadChunks()
{
	int x = player->pos.x;
	int z = player->pos.z;

	int pcx = (x + (x<0 ? 1 : 0)) / Chunk::SIZE - (x<0 ? 1 : 0);
	int pcz = (z + (z<0 ? 1 : 0)) / Chunk::SIZE - (z<0 ? 1 : 0);

	std::map<int, std::map<int, std::map<int, bool>>> chunksToRemesh;

	int RENDER_DISTANCE_SQUARED = RENDER_DISTANCE * RENDER_DISTANCE;


	for(int dx = - RENDER_DISTANCE; dx <= RENDER_DISTANCE; dx++)
	{
		int dx_squared = dx * dx;

		for(int dz =  -RENDER_DISTANCE; dz <= RENDER_DISTANCE; dz++)
		{
			if((dx_squared + dz*dz)>RENDER_DISTANCE_SQUARED)
				continue;

			int distance = std::abs(dx) + std::abs(dz);

			int cx = pcx + dx;
			int cz = pcz + dz;

			if(!isChunkLoaded(cx, 0, cz))
			{
				chunkProvider->requestChunk(cx, cz, distance);
			}
		}
	}
	chunkProvider->update();

	auto available = chunkProvider->getAvailableChunks();

	for(unsigned int i = 0; i < available.size(); i++)
	{
		auto chunk = available[i];
		int cx = chunk[0]->cx;
		int cz = chunk[0]->cz;

		for(int cy = 0; cy < Chunk::HEIGHT; cy++)
		{
			chunk[cy]->loadToVao();
			chunks[cx][cy][cz] = chunk[cy];
//			chunksToRemesh[cx][cy][cz] = true;
		}
	}
	chunkProvider->availableChunks.clear();
//		chunksToRemesh[ccx][y][ccz] = true;
//		chunksToRemesh[ccx-1][y][ccz] = true;
//		chunksToRemesh[ccx+1][y][ccz] = true;
//		chunksToRemesh[ccx][y-1][ccz] = true;
//		chunksToRemesh[ccx][y+1][ccz] = true;
//		chunksToRemesh[ccx][y][ccz+1] = true;
//		chunksToRemesh[ccx][y][ccz-1] = true;

//	for(auto x : chunksToRemesh)
//	{
//		int cx = x.first;
//		for(auto y : x.second)
//		{
//			int cy = y.first;
//			for(auto z : y.second)
//			{
//				int cz = z.first;
//				if(z.second &&  isChunkLoaded(cx, cy, cz))
//				{
//					Chunk * chunk = getChunk(cx, cy, cz);
//					chunk->generateMesh();
//				}
//			}
//		}
//	}
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
	player->move(dt);
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

	chunk->setBlock((char)chunkCoords.x, (char)chunkCoords.y, (char)chunkCoords.z, id);
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
