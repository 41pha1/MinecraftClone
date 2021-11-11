#include "ChunkProvider.h"

#include <future>
#include <thread>
#include <iostream>
#include <string>

std::vector<Chunk*> ChunkProvider::getAvailableChunks()
{
	return availableChunks;
}

struct threadData
{
	std::array<int, 3> coord;
	Chunk * savePointer;
};

//DWORD WINAPI chunkloadThread(LPVOID lpParameter)
//{
//	auto prms = (std::promise<std::string> *) lpParameter;
//	prms->set_value("hello from future");
//	//std::cout << "loading chunk " << data->coord[0] << ", " << data->coord[1] << ", " << data->coord[2] <<  std::endl;
//
//	return 0;
//}

void ChunkProvider::update()
{
	if(!threadRunning)
	{
		std::cout << "update" << std::endl;
		if(requestedChunks.size() > 0)
		{
//			CloseHandle(threadHandle);
			threadRunning = true;
			threadData data;
			data.coord = requestedChunks[0];

//			std::promise<std::string> prms;
//			std::future<std::string> ftr = prms.get_future();

			std::cout << data.coord[0] << ", " << data.coord[1] << ", " << data.coord[2] << std::endl;
			data.savePointer = 0;

//			threadHandle = (HANDLE) CreateThread(NULL,0,chunkloadThread, &prms, 0,NULL);
		}
	}
}

void ChunkProvider::requestChunk(int cx, int cy, int cz)
{
	std::array<int, 3> requestedChunk = {cx, cy, cz};
	requestedChunks.push_back(requestedChunk);
}
