#ifndef GAME_H_
#define GAME_H_

#include <glm/ext/vector_float3.hpp>
#include <map>
#include <string>
#include <vector>

#include "Chunk.h"
#include "MinecraftWorldLoader.h"
#include "ChunkProvider.h"
#include "WorldGenerator.h"
#include "Camera.h"

class Game
{
public:
	static const int RENDER_DISTANCE = 3 ;

	std::map<int, std::map<int, std::map<int, Chunk*>>> chunks = {};
	std::vector<Chunk*> renderQueue = {};
	std::vector<glm::vec3> updateQueue;
	std::string worldFolder;
	bool loadWorld = false;
	Camera * camera;
	Player * player;
	WorldGenerator * generator;
	MinecraftWorldLoader * wLoader;
	ChunkProvider * chunkProvider;
	int gametick = 0;

	Chunk * invalidChunk = new Chunk();
	char invalidBlock = -1;

	Game(Camera *, std::string worldFolder = "");
	virtual ~Game();
	void update(float dt);
	void tick();

	std::array<Chunk *, Chunk::HEIGHT>* loadChunk(int cx, int cz);
	glm::vec3 toChunkCoords(int x, int y, int z);
	void loadChunks();
	bool isChunkLoaded(int cx, int cy, int cz);
	void updateRenderQueue();
	char getBlock(int x, int y, int z);
	void setBlock(int x, int y, int z, int id);
	void placeBlock(int x, int y, int z, int id);
	Chunk * getChunk(int cx, int cy, int cz);
};

#endif
