#ifndef BLOCK_H_
#define BLOCK_H_

#include <map>
#include <string>

class Game;
class Block
{
public:
	static const char UNKNOWN = -1, AIR = 0, GRASS_BLOCK = 1, DIRT = 2, COBBLESTONE = 3, STONE = 4, OAK_PLANK = 5, BRICK = 6, BEDROCK = 7, SAND = 8, GRAVEL = 9, OAK_LOG = 10, IRON_BLOCK = 11, GOLD_BLOCK = 12, DIAMOND_BLOCK = 13, TNT = 14,
	SNOW = 15, LEAF = 16, ICE = 17, CLAY = 18, SPRUCE_LOG = 19, OBSIDIAN = 20, SPONGE = 21, GLASS = 22, GOLD_ORE = 23, IRON_ORE = 24, COAL_ORE = 25, DIAMOND_ORE = 26, REDSTONE_ORE = 27, GRASS = 28, RED_MUSHROOM = 29, BROWN_MUSHROOM = 30,
	POPPY = 31, DANDELION = 32, CRAFTING_BENCH = 33, STONE_BRICK = 34, SNOW_BLOCK = 35, TALL_GRASS = 36, FERN = 37, LARGE_FERN = 38, LAPIS_LAZULI = 39, ANDESITE = 40, GRANITE = 41, DIORITE = 42, LAPIS_ORE = 43, MELON = 44,
	COARSE_DIRT = 45,CHISELED_QUARTZ_BLOCK = 46, DARK_PRISMARINE = 47, PRISMARINE_BRICKS = 48, POLISHED_DIORITE = 49, QUARTZ_BRICKS = 50, QUARTZ_PILLAR = 51, SEA_LANTERN = 52, CYAN_CONCRETE = 53, MOSSY_COBBLESTONE = 54, CRACKED_STONE_BRICKS = 55,
	QUARTZ_BLOCK = 56;
	static const int TOP = 4, BOTTOM = 5, NORTH = 0, SOUTH = 1, WEST = 3, EAST = 2;
	static const int BLOCK = 0, FOLIAGE = 2;
	static float ** vertices;
	static float ** normals;
	static float ** uvs;
	static std::map<std::string, int> nameToID;

	static constexpr const int indices[] = {0,1,2,0,3,1,4,5,6,4,7,5,8,9,10,8,11,9,12,13,14,12,15,13,16,17,18,16,19,17,20,21,22,20,23,21,24,25,26,24,27,25};

	static constexpr const float foliageVertices[] = {
			1.0f, .0f, .0f,
			0.0f, 1.0f, 1.0f,
			0.0f, 0.0f, 1.0f,
			1.0f, 1.0f,0.0f,

			0.0f, 1.0f, 1.0f,
			1.0f, .0f, .0f,
			0.0f, 0.0f, 1.0f,
			1.0f, 1.0f,0.0f,

			1.0f, .0f, 1.0f,
			0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,
			1.0f, 1.0f, 1.0f,

			0.0f, 0.0f, 0.0f,
			1.0f, .0f, 1.0f,
			0.0f, 1.0f, 0.0f,
			1.0f, 1.0f, 1.0f};

	static constexpr const float foliageUVs[] = {
			1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f,
			0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,
			1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f};

	static constexpr const float foliageNormals[] = {
			0.0f, 1.0f, 0.0f,0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,0.0f, 1.0f, 0.0f,
			0.0f, 1.0f, 0.0f,0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,0.0f, 1.0f, 0.0f,
			0.0f, 1.0f, 0.0f,0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,0.0f, 1.0f, 0.0f,
			0.0f, 1.0f, 0.0f,0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,0.0f, 1.0f, 0.0f};

	static constexpr const int foliageIndices[] = {0,1,2,0,3,1,4,5,6,7,5,4,8,9,10,8,10,11,12,13,14,14,13,15};

	static void generateMeshData();

	Block();
	Block(int id);

	static const float * getVertices(char id, int visibility);
	static const float * getUVs(char id, int visibility);
	static const float * getNormals(char id, int visibility);
	static const int * getIndices(char id, int visibility);
	static void init();
	static void setNameToID();
	static bool isTransparent(char id);
	static bool isRenderable(char id);
	static bool hasBlockCollision(char id);
	static int getMeshType(char id);
	static int getVertexCount(char id, int v);
	static int getIndexCount(char id, int v);
	static void update(char id, Game * game, int x, int y, int z);
	static void destroy(char id, Game * game, int x, int y, int z);
};

#endif
