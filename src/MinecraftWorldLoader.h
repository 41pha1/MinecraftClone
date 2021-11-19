#ifndef MINECRAFTWORLDLOADER_H_
#define MINECRAFTWORLDLOADER_H_

#include <map>
#include <string>
#include <vector>

class BlockPalette;
class Chunk;
class Game;

class NBTData;

class MinecraftWorldLoader
{
public:
	Game * game;
	std::map<int, std::map<int, std::vector<char>*>> regionFiles;

	MinecraftWorldLoader(Game * game);
	void loadWorld(std::string fileLocation);
	void loadRegion(std::string regionFileLocation);
	Chunk * loadChunk(std::vector<char> &bytes, long index);
	Chunk * loadChunk(int cx, int cz, std::string worldFolder);
	Chunk * newEmptyChunk(int cx, int cz);
	BlockPalette * loadSection(NBTData * section);
	std::vector<char> inflateZlib(std::vector<char> &bytes);
	virtual ~MinecraftWorldLoader();
};

#endif /* MINECRAFTWORLDLOADER_H_ */
