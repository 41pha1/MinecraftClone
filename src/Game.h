#ifndef GAME_H_
#define GAME_H_

#include <glm/ext/vector_float3.hpp>
#include <map>
#include <string>
#include <vector>

#include "Chunk.h"

class Camera;
class ChunkProvider;
class MinecraftWorldLoader;
class Player;
class WorldGenerator;

class Game
{
public:
	static const int RENDER_DISTANCE = 8;

	std::map<int, std::map<int, Chunk*>> chunks = {};
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
	int loadedChunks = 0;

	Chunk * invalidChunk;
	char invalidBlock = -1;

	Game(Camera *, std::string worldFolder = "");
	virtual ~Game();
	void update(float dt);
	void tick();

	Chunk * loadChunk(int cx, int cz);
	glm::vec3 toChunkCoords(int x, int y, int z);
	void loadChunks();
	void unloadChunks();
	bool isChunkLoaded(int cx, int cz);
	void updateRenderQueue();
	char getBlock(int x, int y, int z);
	void setBlock(int x, int y, int z, int id);
	void placeBlock(int x, int y, int z, int id);
	Chunk * getChunk(int cx, int cz);
};

#endif
