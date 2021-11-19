#include "MinecraftWorldLoader.h"

#include <sys/time.h>
#include <zconf.h>
#include <zlib.h>
#include <algorithm>
#include <array>
#include <cmath>
#include <fstream>
#include <iostream>
#include <iterator>

#include "Block.h"
#include "BlockPalette.h"
#include "Chunk.h"
#include "NBTData.h"
#include "NBTParser.h"
#include "Subchunk.h"

std::map<std::string, int> Block::nameToID;

MinecraftWorldLoader::MinecraftWorldLoader(Game * game_)
{
	game = game_;
}

void MinecraftWorldLoader::loadWorld(std::string fileLocation)
{

}

void MinecraftWorldLoader::loadRegion(std::string regionFileLocation)
{
	std::ifstream input(regionFileLocation, std::ios::binary);

	std::vector<char> bytes(
		 (std::istreambuf_iterator<char>(input)),
		 (std::istreambuf_iterator<char>()));

	input.close();

	for(int i = 0; i < 1; i++)
	{
		int offset = int((unsigned char)(bytes[i*4]) << 16 |
		            (unsigned char)(bytes[i*4 + 1]) << 8 |
		            (unsigned char)(bytes[i*4 + 2]));

		int sectorCount = bytes[i*4 + 3];
//		int cx = (int)(i & 31);
//		int cz = (int)(std::floor(i / 32));

		if(sectorCount != 0)
			loadChunk(bytes, offset * 4096);
	}
}

Chunk* MinecraftWorldLoader::newEmptyChunk(int cx, int cz)
{
	auto subchunks = new std::array<Subchunk*, Chunk::HEIGHT>();
	auto chunk =  new Chunk(game, subchunks, cx, cz);

	for(int cy = 0; cy < Chunk::HEIGHT; cy ++)
		subchunks->operator [](cy) = new Subchunk(chunk, new BlockPalette(Chunk::SIZE), cx, cy, cz);

	return chunk;
}

Chunk * MinecraftWorldLoader::loadChunk(int cx, int cz, std::string worldFolder)
{
	int rx = std::floor(cx/32.0);
	int rz = std::floor(cz/32.0);

	std::vector<char>* bytes;

	if(regionFiles.count(rx) != 0 && regionFiles[rx].count(rz) != 0)
	{
		bytes = regionFiles[rx][rz];
		if(bytes == 0)
			return newEmptyChunk(cx, cz);
	}
	else
	{
		std::string regionFileLocation = worldFolder + "/region/r." + std::to_string(rx) + "." + std::to_string(rz) + ".mca";
		std::cout << regionFileLocation << std::endl;
		std::ifstream input(regionFileLocation, std::ios::binary);
		if(input.fail())
		{
			regionFiles[rx][rz] = 0;
			return newEmptyChunk(cx, cz);
		}

		bytes = new std::vector<char>((std::istreambuf_iterator<char>(input)), (std::istreambuf_iterator<char>()));
		regionFiles[rx][rz] = bytes;
		input.close();
	}
	cx &= 31;
	cz &= 31;
	int i = cx + cz * 32;
	int offset = int((unsigned char)(bytes->operator [](i*4)) << 16 | (unsigned char)(bytes->operator [](i*4+1)) << 8 | (unsigned char)(bytes->operator [](i*4+2)));

	int sectorCount = bytes->operator [](i*4+3);

	if(sectorCount != 0)
		return loadChunk(*bytes, offset * 4096);

	return newEmptyChunk(cx, cz);
}


Chunk * MinecraftWorldLoader::loadChunk(std::vector<char> &bytes, long index)
{
	int length = int((unsigned char)(bytes[index]) << 24 |
					(unsigned char)(bytes[index + 1]) << 16 |
					(unsigned char)(bytes[index + 2]) << 8 |
					(unsigned char)(bytes[index + 3]));
	int compression = int((unsigned char)bytes[index + 4]);

	std::vector<char>::const_iterator first = bytes.begin() + index + 5;
	std::vector<char>::const_iterator last = bytes.begin() + index + length + 4;

	std::vector<char> data(first, last);

	if(compression == 2)
		data = inflateZlib(data);

	else if(compression != 3)
		std::cerr << "Unsupported compression in region-file" << std::endl;

	NBTData * chunkData = NBTParser::parseData(data);
	NBTData * levelData = chunkData->get("Level");

	int cx = *(int*) levelData->get("xPos")->data;
	int cz = *(int*) levelData->get("zPos")->data;

	auto subchunks = new std::array<Subchunk*, Chunk::HEIGHT>();
	auto sections =  *(std::vector<NBTData *>*) levelData->get("Sections")->data;

	auto chunk = new Chunk(game, subchunks, cx, cz);

	for(NBTData * section : sections)
	{
		char cy = *(char*)section->get("Y")->data;

		if(cy >= 0 && cy < Chunk::HEIGHT && section->get("BlockStates") != 0)
		{
			auto blocks = loadSection(section);
			(*subchunks)[cy] = new Subchunk(chunk, blocks, cx, cy, cz);
		}
	}

	for(int cy = 0; cy < Chunk::HEIGHT; cy ++)
	{
		if((*subchunks)[cy] == 0)
			(*subchunks)[cy] = new Subchunk(chunk, new BlockPalette(Chunk::SIZE), cx, cy, cz);
	}

	delete chunkData;
	return chunk;
}

BlockPalette * MinecraftWorldLoader::loadSection(NBTData * section)
{
	auto blocks = new BlockPalette(Chunk::SIZE);
	std::vector<long long> blockStates = *(std::vector<long long> *)section->get("BlockStates")->data;
	std::vector<NBTData *> palette = *(std::vector<NBTData *> *)section->get("Palette")->data;
	std::vector<std::string> blockIDs(palette.size());
	std::transform(palette.begin(), palette.end(), blockIDs.begin(), [](NBTData * data) -> std::string { return *(std::string*) data->get("Name")->data; });

	int bitsPerIndex = std::max(4, 32 -__builtin_clz(palette.size()-1));
	int indicesPerLong = int(64 / bitsPerIndex);

	int x = 0, z = 0, y = 0;

	for(long long states : blockStates)
	{
		for(int i = 0; i < indicesPerLong; i++)
		{
			int index = (((long long) states) >> ((i * bitsPerIndex))) & ((1 << bitsPerIndex)-1);
			if((unsigned int) index>=palette.size())
			{
				//std::cerr << "Invalid palette entry!" << std::endl;
				index = 0;
			}
//			std::cout << bitsPerIndex << " | " << x << ", " << y << ", " << z << ", " << blockIDs[index] << std::endl;
			int id = Block::nameToID[blockIDs[index]];
			if(id == 0 && (blockIDs[index].compare("minecraft:cave_air")) && (blockIDs[index].compare("minecraft:air")))
			{
//				std::cerr << "Unkown block id " << blockIDs[index] << std::endl;
			}

			blocks->set(x,y,z,id);

			x ++;
			if(x % 16 == 0){
				x = 0;
				z ++;

				if(z % 16 == 0){
					z = 0;
					y ++;
//					std::cout << std::endl;
					if(y % 16 == 0)
					{
						return blocks;
					}
				}
			}

		}
	}
	return blocks;
}

std::vector<char> MinecraftWorldLoader::inflateZlib(std::vector<char>& bytes)
{
    char b[bytes.size()];
    std::copy(bytes.begin(), bytes.end(), b);

	char c[500000]; //TODO

	z_stream infstream;
	infstream.zalloc = Z_NULL;
	infstream.zfree = Z_NULL;
	infstream.opaque = Z_NULL;
    infstream.avail_in = (uInt)(bytes.size()); // size of input
	infstream.next_in = (Bytef *)b; // input char array
	infstream.avail_out = (uInt)sizeof(c); // size of output
	infstream.next_out = (Bytef *)c; // output char array

	inflateInit(&infstream);
	int res = inflate(&infstream, Z_FINISH);
	inflateEnd(&infstream);

	int inflatedLength = (uInt)((char*)infstream.next_out - c);

	if(res < 0)
		std::cerr << "Decompression error!" << std::endl;

	return std::vector<char> (c, c + inflatedLength);
}

MinecraftWorldLoader::~MinecraftWorldLoader()
{

}
