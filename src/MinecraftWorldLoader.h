#ifndef MINECRAFTWORLDLOADER_H_
#define MINECRAFTWORLDLOADER_H_

#include <array>
#include <string>
#include <vector>

#include "Chunk.h"

class Chunk;

class MinecraftWorldLoader
{
public:
	Game * game;
	MinecraftWorldLoader(Game * game);
	static void loadWorld(std::string fileLocation);
	static void loadRegion(std::string regionFileLocation);
	static std::array<Chunk*, 16> loadChunk(std::vector<char> &bytes, long index);
	static std::vector<char> inflateZlib(std::vector<char> &bytes);
	virtual ~MinecraftWorldLoader();
};

#endif /* MINECRAFTWORLDLOADER_H_ */
