#ifndef CHUNKPROVIDER_H_
#define CHUNKPROVIDER_H_

#include <winnt.h>
#include <map>
#include <vector>

#include "Chunk.h"

struct threadInfo
{
	bool threadRunning = false;
	std::array<Chunk *, Chunk::HEIGHT>** returnPointer = 0;
	HANDLE threadHandle = 0;
};


class ChunkProvider
{
static const int MAX_THREADS = 8;

public:
	Game * game;
	std::array<threadInfo, MAX_THREADS> threads;
	ChunkProvider(Game * game);
    std::vector<std::array<Chunk *, Chunk::HEIGHT>> availableChunks;
	std::map<int, std::map<int, bool>> isChunkRequested;
	std::vector<std::array<int, 3>> requestedChunks;

	std::vector<std::array<Chunk *, Chunk::HEIGHT>> getAvailableChunks();
	void update();
	void requestChunk(int cx, int cz, int distance);
};

#endif /* CHUNKPROVIDER_H_ */
