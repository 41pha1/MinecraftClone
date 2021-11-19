#include "ChunkProvider.h"

#include <winbase.h>
#include <windef.h>
#include <algorithm>
#include <iostream>
#include <string>


 std::vector<Chunk*> ChunkProvider::getAvailableChunks()
{
	return availableChunks;
}

struct threadData
{
	std::array<int, 2> coord;
	Game * game;
	Chunk** savePointer;
};

struct meshThreadData
{
	Chunk * chunk;
};

DWORD WINAPI chunkloadThread(LPVOID lpParameter)
{
	auto data = (threadData *) lpParameter;
	Chunk * chunk = data->game->loadChunk(data->coord[0], data->coord[1]);
	*data->savePointer = chunk;
	return 0;
}

DWORD WINAPI chunkMeshThread(LPVOID lpParameter)
{
	auto data = (meshThreadData *) lpParameter;
	data->chunk->generateMesh();
	return 0;
}

ChunkProvider::ChunkProvider(Game* game_)
{
	game = game_;
}

void ChunkProvider::update(int pcx, int pcz)
{
	for(auto & chunkCoord : requestedChunks)
		chunkCoord[2] = (chunkCoord[0] - pcx)*(chunkCoord[0] - pcx) + (chunkCoord[1] - pcz)*(chunkCoord[1] - pcz);

	std::sort(requestedChunks.begin(), requestedChunks.end(), [](std::array<int, 3> a, std::array<int, 3> b) -> bool{return a[2]<b[2];});

	for(auto & chunkMesh : requestedChunkMeshes)
		chunkMesh.distance = (chunkMesh.chunk->cx - pcx)*(chunkMesh.chunk->cx - pcx) + (chunkMesh.chunk->cz - pcz)*(chunkMesh.chunk->cz - pcz);

	std::sort(requestedChunkMeshes.begin(), requestedChunkMeshes.end(), [](requestedMesh a, requestedMesh b) -> bool{return a.distance<b.distance;});

	for(int i = 0; i < MAX_THREADS; i++)
	{
		if(threads[i].threadRunning && WaitForSingleObject(threads[i].threadHandle, 0) == WAIT_OBJECT_0)
		{
			threads[i].threadRunning = false;
			CloseHandle(threads[i].threadHandle);
			availableChunks.push_back(*threads[i].returnPointer);
		}

		if(meshingThreads[i].threadRunning && WaitForSingleObject(meshingThreads[i].threadHandle, 0) == WAIT_OBJECT_0)
		{
			meshingThreads[i].threadRunning = false;
			CloseHandle(meshingThreads[i].threadHandle);
			meshedChunks.push_back(meshingThreads[i].meshedChunk);
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
				data->savePointer = new (Chunk*);
				data->game = game;

				threads[i].returnPointer = data->savePointer;
				requestedChunks.erase(requestedChunks.begin());

				threads[i].threadHandle = (HANDLE) CreateThread(NULL,0,chunkloadThread, data, 0,NULL);
				threads[i].threadRunning = true;
			}
		}

		if(requestedChunkMeshes.size() > 0)
		{
			if(!meshingThreads[i].threadRunning)
			{
				meshThreadData * data = new meshThreadData();
				data->chunk = requestedChunkMeshes[0].chunk;

				meshingThreads[i].meshedChunk = requestedChunkMeshes[0].chunk;
				requestedChunkMeshes.erase(requestedChunkMeshes.begin());

				meshingThreads[i].threadHandle = (HANDLE) CreateThread(NULL,0,chunkMeshThread, data, 0,NULL);
				meshingThreads[i].threadRunning = true;
			}
		}
	}
}

std::vector<Chunk*> ChunkProvider::getAvailableMeshes()
{
	return meshedChunks;
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

void ChunkProvider::requestChunkMesh(Chunk* chunk)
{
	if(isChunkMeshingRequested.count(chunk->cx) == 0)
		isChunkMeshingRequested[chunk->cx] = std::map<int, bool>();

	if(!isChunkMeshingRequested[chunk->cx][chunk->cz])
	{
		isChunkMeshingRequested[chunk->cx][chunk->cz] = true;
		requestedMesh  req = requestedMesh();
		req.chunk = chunk;
		req.distance = 0;
		requestedChunkMeshes.push_back(req);
	}
}
