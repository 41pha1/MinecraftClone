#ifndef MINECRAFTWORLDLOADER_H_
#define MINECRAFTWORLDLOADER_H_

#include <array>
#include <map>
#include <string>
#include <vector>

#include "Chunk.h"

class NBTData;

class MinecraftWorldLoader
{
public:
	Game * game;
	std::map<int, std::map<int, std::vector<char>*>> regionFiles;

	MinecraftWorldLoader(Game * game);
	void loadWorld(std::string fileLocation);
	void loadRegion(std::string regionFileLocation);
	std::array<Chunk*, Chunk::HEIGHT>* loadChunk(std::vector<char> &bytes, long index);
	std::array<Chunk*, Chunk::HEIGHT>* loadChunk(int cx, int cz, std::string worldFolder);
	char *** loadSection(NBTData * section);
	std::vector<char> inflateZlib(std::vector<char> &bytes);
	virtual ~MinecraftWorldLoader();
};

#endif /* MINECRAFTWORLDLOADER_H_ */
