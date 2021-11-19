#ifndef CHUNKPROVIDER_H_
#define CHUNKPROVIDER_H_

#include <winnt.h>
#include <array>
#include <map>
#include <vector>
#include "Game.h"
#include "Chunk.h"

struct threadInfo
{
	bool threadRunning = false;
	Chunk** returnPointer = 0;
	HANDLE threadHandle = 0;
};

struct meshingThreadInfo
{
	bool threadRunning = false;
	HANDLE threadHandle = 0;
	Chunk * meshedChunk = 0;
};

struct requestedMesh
{
	Chunk * chunk;
	int distance;
};

class ChunkProvider
{
public:
	static const int MAX_THREADS = 8;

	Game * game;
	std::array<threadInfo, MAX_THREADS> threads;
	ChunkProvider(Game * game);
    std::vector<Chunk *> availableChunks;
	std::map<int, std::map<int, bool>> isChunkRequested;
	std::vector<std::array<int, 3>> requestedChunks;

	std::array<meshingThreadInfo, MAX_THREADS> meshingThreads;
    std::vector<Chunk *> meshedChunks;
	std::map<int, std::map<int, bool>> isChunkMeshingRequested;
	std::vector<requestedMesh> requestedChunkMeshes;

	std::vector<Chunk *> getAvailableChunks();
	std::vector<Chunk *> getAvailableMeshes();
	void update(int pcx, int pcz);
	void requestChunk(int cx, int cz, int distance);
	void requestChunkMesh(Chunk *);
};

#endif /* CHUNKPROVIDER_H_ */
