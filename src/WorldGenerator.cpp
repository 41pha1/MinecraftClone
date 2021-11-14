#include "WorldGenerator.h"

#include <algorithm>
#include <array>
#include <cstdlib>
#include <sstream>
#include <string>
#include <unordered_map>

#include "Block.h"
#include "Chunk.h"

int blockcount = 0;

WorldGenerator::WorldGenerator(std::size_t SEED_SEED)
{
	SEED = std::hash<std::size_t>()(SEED_SEED);
	SimplexNoise::setSeed(SEED);
}

std::size_t WorldGenerator::calculateChunkSeed(int cx, int cy, int cz)
{
	std::stringstream ss;
	ss << SEED << "," << cx << "," << cy << "," << cz;
	std::string str = ss.str();
	return std::hash<std::string>()(str);
}

void WorldGenerator::generateTree(Chunk * chunk, int x, int y, int z, int type)
{
	int trunkHeight = 4 + (3*rand()) / RAND_MAX;
	int coneRadius =  (10 + (5*rand()) / RAND_MAX)  / trunkHeight;

	for(int dx = -coneRadius; dx <= coneRadius; dx++)
	{
		for(int dy = -coneRadius; dy <= coneRadius; dy++)
		{
			for(int dz = -coneRadius; dz <= coneRadius; dz++)
			{
				int bx = x + dx;
				int by = y + dy + trunkHeight;
				int bz = z +dz;
				int d =  dx*dx +dy*dy +dz*dz;
				if((rand()*coneRadius*(float)d) < (RAND_MAX*10))
					if(chunk->getBlock(bx, by, bz) == Block::AIR)
						chunk->setBlock(bx, by, bz, Block::LEAF);
			}
		}
	}


	for(int dy = 0; dy < trunkHeight; dy++)
	{
		chunk->setBlock(x, y+dy, z, Block::OAK_LOG);
	}
}

void WorldGenerator::populateTerrain(Chunk * chunk)
{
	long long int chunkSeed = calculateChunkSeed(chunk->cx, chunk->cy, chunk->cz);
	srand((int)chunkSeed);

	int size = Chunk::SIZE;

	for(int x = 0; x < size; x++)
	{
//		int wx = x + chunk->cx * Chunk::SIZE;
		for(int z = 0; z < size; z++)
		{
//			int wz = z + chunk->cz * Chunk::SIZE;

			for(int y = 0; y < size; y++)
			{
				int wy = y + chunk->cy * Chunk::SIZE;

				if(chunk->getBlock(x, y-1, z) == Block::GRASS_BLOCK && chunk->getBlock(x, y, z) == Block::AIR )
				{
					if(rand() < 10000 - (wy * 100.))
						chunk->setBlock(x,y,z,Block::GRASS);

					if(rand() < 300)
						chunk->setBlock(x,y,z,Block::RED_FLOWER);

					if(rand() < 300)
						chunk->setBlock(x,y,z,Block::YELLOW_FLOWER);

					if(rand() < 300)
						chunk->setBlock(x,y,z,Block::FERN);

					if(rand() < 300)
						generateTree(chunk, x,y,z, 0);
				}
			}
		}
	}
}

bool WorldGenerator::doesHeightMapExist(int cx, int cz)
{
	if(heightMaps.count(cx) == 0)
		return false;
	if(heightMaps[cx].count(cz) == 0)
		return false;

	return heightMaps[cx][cz] != NULL;
}

void WorldGenerator::generareHeightMap(int cx, int cz)
{
	int size = Chunk::SIZE;
	float * map = (float*)malloc(size*size*NUM_HEIGHT_MAPS*sizeof(float));
	for(int x = 0; x < size; x++)
	{
		int wx = x + cx * size;
		for(int z = 0; z < size; z++)
		{
			int wz = z + cz * size;

			std::array<float, NUM_PROPERTIES> biome;
			biome[AMPLITUDE] = std::min(1., std::abs(wx) / 500.f + 0.01);
			biome[ROUGHNESS] = 0.3;
			biome[TEMPERATURE] = 0.5;
			biome[HEIGHT] = 0.3;

			SimplexNoise terrainNoise = SimplexNoise(0.002f, 1, 2.0, 0.25 + biome[ROUGHNESS]*0.4);
			float terrainHeight = (terrainNoise.fractal(8, wx, wz)*biome[AMPLITUDE]*100. + TERRAIN_LEVEL) + biome[HEIGHT]*50.f;

			float temperature = 2.f + (biome[TEMPERATURE]-0.5)*6.f - (terrainHeight-TERRAIN_LEVEL) * 0.04;

			SimplexNoise snowNoise = SimplexNoise(0.1f, 1, 2.0, 0.2);
			float snowHeight = snowNoise.fractal(3, wx, wz) - temperature;
			if(snowHeight < 0)
				snowHeight = 0;

			SimplexNoise iceNoise = SimplexNoise(0.1f, 1, 2.0, 0.2);
			float iceHeight = (iceNoise.fractal(3, wx, wz) - temperature) * 0.4f;
			if(iceHeight < 0)
				iceHeight = 0;

			SimplexNoise dirtNoise = SimplexNoise(0.01f, 1, 2.0, 0.7);
			float dirtHeight = 5.f + dirtNoise.fractal(1, wx+100., wz+100.) * 2.f;
			dirtHeight -= (terrainHeight-TERRAIN_LEVEL)*0.08f + biome[AMPLITUDE];
			if(dirtHeight < 0)
				dirtHeight = 0;

			int index = (x *size + z) * NUM_HEIGHT_MAPS;
			map[index + TERRAIN_HEIGHT] = terrainHeight;
			map[index + SNOW_HEIGHT] = snowHeight;
			map[index + ICE_HEIGHT] = iceHeight;
			map[index + DIRT_HEIGHT] = dirtHeight;
		}
	}
	heightMaps[cx][cz] = map;
}

void WorldGenerator::generateTerrain(Chunk * chunk)
{
	int size = Chunk::SIZE;

	if(!doesHeightMapExist(chunk->cx, chunk->cz))
		generareHeightMap(chunk->cx, chunk->cz);

	char *** blocks = new char**[size];
	for(int x = 0; x < size; x++)
	{
		blocks[x] = new char*[size];
		for(int y = 0; y < size; y++)
		{
			blocks[x][y] = new char[size];
		}
	}

	for(int x = 0; x < size; x++)
	{
		for(int z = 0; z < size; z++)
		{

			int index = (x *size + z) * NUM_HEIGHT_MAPS;
			float terrainHeight = heightMaps[chunk->cx][chunk->cz][index + TERRAIN_HEIGHT];
			float snowHeight = heightMaps[chunk->cx][chunk->cz][index + SNOW_HEIGHT];
			float iceHeight = heightMaps[chunk->cx][chunk->cz][index + ICE_HEIGHT];
			float dirtHeight = heightMaps[chunk->cx][chunk->cz][index + DIRT_HEIGHT];

			for(int y = 0; y < size; y++)
			{
				blocks[x][y][z] = Block::AIR;
				blockcount++;

				int wy = y + chunk->cy * size;

				if(wy < terrainHeight + snowHeight)
					blocks[x][y][z] = Block::SNOW_BLOCK;

				if(wy < terrainHeight + iceHeight)
					blocks[x][y][z] = Block::ICE;
				if(wy < (terrainHeight))
					blocks[x][y][z] = Block::GRASS_BLOCK;

				if(wy < (terrainHeight) && snowHeight > 0)
					blocks[x][y][z] = Block::SNOW;

				if(wy < (terrainHeight -1.))
					blocks[x][y][z] = Block::DIRT;

				if(wy < (terrainHeight - dirtHeight))
					blocks[x][y][z] = Block::STONE;

				if(wy > terrainHeight && wy < SEA_LEVEL)
					blocks[x][y][z] = Block::ICE;

				if(wy == 0)
					blocks[x][y][z] = Block::BEDROCK;
			}
		}
	}
	chunk->blocks->set(blocks);
}

WorldGenerator::~WorldGenerator()
{
}

