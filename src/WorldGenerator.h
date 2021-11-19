#ifndef WORLDGENERATOR_H_
#define WORLDGENERATOR_H_

#include <cstddef>
#include <map>

#include "Simplex.h"

class Chunk;

class WorldGenerator
{
public:
	static const int AMPLITUDE = 0, ROUGHNESS = 1,  WETNESS = 2, TEMPERATURE = 3, FORESTATION = 4, HEIGHT = 5;
	static const int TERRAIN_HEIGHT = 0, SNOW_HEIGHT = 1, ICE_HEIGHT = 2, DIRT_HEIGHT = 3;
	static const int NUM_PROPERTIES = 6, NUM_HEIGHT_MAPS = 4;
	static const int SEA_LEVEL = 60;
	static const int TERRAIN_LEVEL = 60;
	std::size_t SEED;
	SimplexNoise noise;
	std::map<int, std::map<int, float*>> heightMaps;
	WorldGenerator(std::size_t seed);
	std::size_t calculateChunkSeed(int cx, int cz);
	void generateTree(Chunk * chunk, int x, int y, int z, int type);
	void generateTerrain(Chunk * chunk);
	void populateTerrain(Chunk * chunk);
	float * generareHeightMap(int cx, int cz);
	virtual ~WorldGenerator();
};

#endif /* WORLDGENERATOR_H_ */
