#include "ChunkProvider.h"

#include <winbase.h>
#include <windef.h>
#include <algorithm>
#include <array>
#include <string>

#include "Game.h"

 std::vector<std::array<Chunk *, Chunk::HEIGHT>> ChunkProvider::getAvailableChunks()
{
	return availableChunks;
}

struct threadData
{
	std::array<int, 2> coord;
	Game * game;
	std::array<Chunk *, Chunk::HEIGHT>** savePointer;
};

DWORD WINAPI chunkloadThread(LPVOID lpParameter)
{
	auto data = (threadData *) lpParameter;
	auto chunk = data->game->loadChunk(data->coord[0], data->coord[1]);
	for(int i = 0; i < Chunk::HEIGHT; i++)
	{
		(*chunk)[i]->generateMesh();
	}

//	std::cout << (*chunk)[0]->cx << ",  " << (*chunk)[0]->cz << ", "  << (*chunk)[0]->vCount <<std::endl;
//	std::cout << "verts: ";
//	for(int i = 0; i < (*chunk)[0]->vCount / 10; i++)
//		std::cout << (*chunk)[0]->verts[i] << " ";
//	std::cout << std::endl;

	*data->savePointer = chunk;
//	std::cout << "end thread" << std::endl;
	return 0;
}

ChunkProvider::ChunkProvider(Game* game_)
{
	game = game_;
}

void ChunkProvider::update()
{
	std::sort(requestedChunks.begin(), requestedChunks.end(), [](std::array<int, 3> a, std::array<int, 3> b) -> bool {return a[2]<b[2]; });
	for(int i = 0; i < MAX_THREADS; i++)
	{
		if(threads[i].threadRunning && WaitForSingleObject(threads[i].threadHandle, 0) == WAIT_OBJECT_0)
		{
			threads[i].threadRunning = false;
			CloseHandle(threads[i].threadHandle);
			availableChunks.push_back(**threads[i].returnPointer);
		}
	}
	for(int i = 0; i < MAX_THREADS; i++)
	{
		if(requestedChunks.size() > 0)
		{
			if(!threads[i].threadRunning)
			{
				threadData * data = new threadData();
				data->coord = {requestedChunks[0][0], requestedChunks[0][1]};
				data->savePointer = new (std::array<Chunk *, Chunk::HEIGHT>*);
				data->game = game;

				threads[i].returnPointer = data->savePointer;
				requestedChunks.erase(requestedChunks.begin());

				threads[i].threadHandle = (HANDLE) CreateThread(NULL,0,chunkloadThread, data, 0,NULL);
				threads[i].threadRunning = true;
			}
		}
	}
//	requestedChunks.clear();
}

void ChunkProvider::requestChunk(int cx, int cz, int distance)
{
	if(isChunkRequested.count(cx) == 0)
		isChunkRequested[cx] = std::map<int, bool>();

	if(!isChunkRequested[cx][cz])
	{
		isChunkRequested[cx][cz] = true;
		requestedChunks.push_back(std::array<int, 3> {cx, cz, distance});
	}
}
