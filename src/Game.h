#ifndef GAME_H_
#define GAME_H_

#include <glm/ext/vector_float3.hpp>
#include <map>
#include <vector>

#include "Chunk.h"
#include "ChunkProvider.h"

class WorldGenerator;
class Camera;
class Player;

class Game
{
public:
	static const int RENDER_DISTANCE = 5;

	std::map<int, std::map<int, std::map<int, Chunk*>>> chunks = {};
	std::vector<Chunk*> renderQueue = {};
	std::vector<glm::vec3> updateQueue;
	Camera * camera;
	Player * player;
	WorldGenerator * generator;
	ChunkProvider * chunkProvider;
	int gametick = 0;

	Chunk * invalidChunk = new Chunk();
	char invalidBlock = -1;

	Game(Camera *);
	virtual ~Game();
	void update(float dt);
	void tick();

	Chunk * loadChunk(int cx, int cy, int cz);
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
