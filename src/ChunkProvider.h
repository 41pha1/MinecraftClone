#ifndef CHUNKPROVIDER_H_
#define CHUNKPROVIDER_H_

#include <winnt.h>
#include <array>
#include <vector>

#include "Chunk.h"

class ChunkProvider
{

public:
    std::vector<Chunk *> availableChunks;
    bool threadRunning;
	HANDLE threadHandle;

	std::vector<std::array<int, 3>> requestedChunks;

	std::vector<Chunk *> getAvailableChunks();
	void update();
	void requestChunk(int cx, int cy, int cz);
};

#endif /* CHUNKPROVIDER_H_ */
