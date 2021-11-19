#include "Game.h"

#include <glm/detail/type_vec3.hpp>
#include <cstdlib>
#include <iostream>
#include <utility>

#include "Block.h"
#include "Camera.h"
#include "ChunkProvider.h"
#include "MinecraftWorldLoader.h"
#include "Player.h"
#include "WorldGenerator.h"

Game::Game(Camera *  cam, std::string worldFolder_)
{
	cam->game = this;
	invalidChunk = new Chunk();
	generator = new WorldGenerator(1234L);
	player = new Player(this);
	chunks = std::map<int, std::map<int, Chunk *>>();
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
			chunks.operator[](x+px)[z+pz] = chunk;
		}
	}

	std::cout << "meshing chunks..." << std::endl;
	for(auto x : chunks)
	{
		for(auto z : x.second)
		{
			if(z.second->valid)
			{
				z.second->generateMesh();
				z.second->loadToVao();
				renderQueue.push_back(z.second);
			}
		}
	}
	std::cout << "Done!" << std::endl;
	//player->setOnGround();
	camera = cam;
	chunkProvider = new ChunkProvider(this);
}

Chunk *  Game::loadChunk(int cx, int cz)
{
	if(loadWorld)
	{
		return wLoader->loadChunk(cx, cz, worldFolder);
	}
	return new Chunk(this, cx, cz);
}

char Game::getBlock(int x, int y, int z)
{
	if(y < 0 || y >= Chunk::SIZE * Chunk::HEIGHT)
		return Block::AIR;
	int cx = (x + (x<0 ? 1 : 0)) / Chunk::SIZE - (x<0 ? 1 : 0);
	int cz = (z + (z<0 ? 1 : 0)) / Chunk::SIZE - (z<0 ? 1 : 0);

	Chunk * chunk = getChunk(cx, cz);

	if(!chunk->valid)
		return invalidBlock;

	glm::vec3 chunkCoords = toChunkCoords(x, y, z);
	return chunk->getBlock((char)chunkCoords.x, (char)chunkCoords.y, (char)chunkCoords.z);
}

bool Game::isChunkLoaded(int cx, int cz)
{
	if(chunks.count(cx) == 0)
		return false;
	if(chunks[cx].count(cz) == 0)
		return false;

	return chunks[cx][cz] != NULL;
}

void Game::updateRenderQueue()
{
	renderQueue.clear();
	int x = player->pos.x;
	int z = player->pos.z;

	int pcx = (x + (x<0 ? 1 : 0)) / Chunk::SIZE - (x<0 ? 1 : 0);
	int pcz = (z + (z<0 ? 1 : 0)) / Chunk::SIZE - (z<0 ? 1 : 0);

	int RENDER_DISTANCE_SQUARED = RENDER_DISTANCE * RENDER_DISTANCE;
	for(int dx = - RENDER_DISTANCE; dx <= RENDER_DISTANCE; dx++)
	{
		int dx_squared = dx * dx;

		for(int dz = - RENDER_DISTANCE; dz <= RENDER_DISTANCE; dz++)
		{
			if((dx_squared + dz*dz)>RENDER_DISTANCE_SQUARED )
				continue;

			int cx = pcx + dx;
			int cz = pcz + dz;

			if(isChunkLoaded(cx, cz) && getChunk(cx, cz)->valid && getChunk(cx, cz)->vao != 0)
			{
				renderQueue.push_back(getChunk(cx, cz));
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

	int RENDER_DISTANCE_SQUARED = (RENDER_DISTANCE+1) * (RENDER_DISTANCE+1);

	for(int dx = - RENDER_DISTANCE-1; dx <= RENDER_DISTANCE+1; dx++)
	{
		int dx_squared = dx * dx;

		for(int dz =  -RENDER_DISTANCE-1; dz <= RENDER_DISTANCE+1; dz++)
		{
			if((dx_squared + dz*dz)>RENDER_DISTANCE_SQUARED)
				continue;

			int distance = std::abs(dx) + std::abs(dz);

			int cx = pcx + dx;
			int cz = pcz + dz;

			if(!isChunkLoaded(cx, cz))
			{
				chunkProvider->requestChunk(cx, cz, distance);
			}else
			{
				auto chunk = getChunk(cx, cz);
				if(!chunk->meshed)
				{
					if(getChunk(cx+1, cz)->valid && getChunk(cx-1, cz)->valid && getChunk(cx, cz+1)->valid && getChunk(cx, cz-1)->valid)
					chunkProvider->requestChunkMesh(chunk);
				}
			}
		}
	}
	chunkProvider->update(pcx, pcz);

	auto available = chunkProvider->getAvailableChunks();

	for(unsigned int i = 0; i < available.size(); i++)
	{
		auto chunk = available[i];
		int cx = chunk->cx;
		int cz = chunk->cz;

		chunks[cx][cz] = chunk;
	}
	chunkProvider->availableChunks.clear();

	auto meshed = chunkProvider->getAvailableMeshes();
	for(unsigned int i = 0; i < meshed.size(); i++)
	{
		auto chunk = meshed[i];
		chunk->loadToVao();
	}
	chunkProvider->meshedChunks.clear();
}

void Game::unloadChunks()
{
	int x = player->pos.x;
	int z = player->pos.z;

	int pcx = (x + (x<0 ? 1 : 0)) / Chunk::SIZE - (x<0 ? 1 : 0);
	int pcz = (z + (z<0 ? 1 : 0)) / Chunk::SIZE - (z<0 ? 1 : 0);

	for(auto x : chunks)
	{
		for(auto z : x.second)
		{
			int cx = z.second->cx;
			int cz = z.second->cz;

			int dx = cx-pcx;
			int dz = cz-pcz;

			if((dx*dx+dz*dz)>(RENDER_DISTANCE+2)*(RENDER_DISTANCE+2))
			{
				delete z.second;
				chunkProvider->isChunkRequested[cx][cz] = false;
				chunkProvider->isChunkMeshingRequested[cx][cz] = false;
				chunks[cx].erase(cz);
			}
		}
	}
}

Chunk* Game::getChunk(int cx, int cz)
{
	if(isChunkLoaded(cx, cz))
		return chunks[cx][cz];

	return invalidChunk;
}

void Game::update(float dt)
{
	camera->update(player);
	player->move(dt);
	loadChunks();
	unloadChunks();
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
	int by = y;
	int bz = z < 0 ?  (((z) % Chunk::SIZE + Chunk::SIZE) % Chunk::SIZE): z % Chunk::SIZE;
	return glm::vec3(bx, by, bz);
}

void Game::setBlock(int x, int y, int z, int id)
{
	int cx = (x + (x<0 ? 1 : 0)) / Chunk::SIZE - (x<0 ? 1 : 0);
	int cz = (z + (z<0 ? 1 : 0)) / Chunk::SIZE - (z<0 ? 1 : 0);

	Chunk * chunk = getChunk(cx, cz);

	if(!chunk->valid)
		return;

	glm::vec3 chunkCoords = toChunkCoords(x, y, z);

	chunk->setBlock((char)chunkCoords.x, (char)chunkCoords.y, (char)chunkCoords.z, id);
}

void Game::placeBlock(int x, int y, int z, int id)
{
	setBlock(x, y, z, id);
	int cx = (x + (x<0 ? 1 : 0)) / Chunk::SIZE - (x<0 ? 1 : 0);
	int cz = (z + (z<0 ? 1 : 0)) / Chunk::SIZE - (z<0 ? 1 : 0);
	getChunk(cx, cz)->generateMesh();
	getChunk(cx, cz)->loadToVao();

	updateQueue.push_back(glm::vec3(x+1, y, z));
	updateQueue.push_back(glm::vec3(x-1, y, z));
	updateQueue.push_back(glm::vec3(x, y+1, z));
	updateQueue.push_back(glm::vec3(x, y-1, z));
	updateQueue.push_back(glm::vec3(x, y, z+1));
	updateQueue.push_back(glm::vec3(x, y, z-1));

	glm::vec3 chunkCoords = toChunkCoords(x, y, z);

	if(chunkCoords.x == 0){
		getChunk(cx-1, cz)->generateMesh();
		getChunk(cx-1, cz)->loadToVao();
	}
	if(chunkCoords.x== Chunk::SIZE-1){
		getChunk(cx+1, cz)->generateMesh();
		getChunk(cx+1, cz)->loadToVao();
	}
	if(chunkCoords.z== 0){
		getChunk(cx, cz-1)->generateMesh();
		getChunk(cx, cz-1)->loadToVao();
	}
	if(chunkCoords.z == Chunk::SIZE-1){
		getChunk(cx, cz+1)->generateMesh();
		getChunk(cx, cz+1)->loadToVao();
	}
}
