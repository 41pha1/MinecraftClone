#include "MinecraftWorldLoader.h"

#include <zconf.h>
#include <zlib.h>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>

#include "NBTParser.h"


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

std::array<Chunk*, 16> MinecraftWorldLoader::loadChunk(std::vector<char> &bytes, long index)
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

	if(compression != 3)
		std::cerr << "Unsupported compression in region-file" << std::endl;

	NBTData levelData = NBTParser::parseData(data).get("Level");
	int cx = *(int*) levelData.get("xPos")->data;
	int cz = *(int*) levelData.get("zPos")->data;

	auto chunks = std::array<Chunk*, 16>();

	for(NBTData &section : *(std::vector<NBTData>*) levelData.get("Sections")->data)
	{
		char yValue = *(char*)section.get("Y")->data;

		std::cout << int(yValue) << std::endl;
		section.print("");
	}

	return chunks;
}

std::vector<char> MinecraftWorldLoader::inflateZlib(std::vector<char>& bytes)
{
    char b[bytes.size()];
    std::copy(bytes.begin(), bytes.end(), b);

	char c[50000];

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
