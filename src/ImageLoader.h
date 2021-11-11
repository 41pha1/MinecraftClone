#ifndef IMAGELOADER_H_
#define IMAGELOADER_H_

#include <glad/glad.h>
#include <glm/ext/vector_float2.hpp>


class ImageLoader
{
public:
	static const int width = 256, height = 256;
	static const unsigned texRes = 16;
	static const int GRASS_BLOCK_SIDE = 0, GRASS_BLOCK_TOP = 1, DIRT = 2, COBBLESTONE = 3, STONE = 4, OAK_PLANKS = 5, BRICKS = 6, BEDROCK = 7, SAND = 8, GRAVEL = 9, OAK_LOG = 10, OAK_LOG_TOP = 11, IRON_BLOCK = 12, GOLD_BLOCK = 13,
	DIAMOND_BLOCK = 14, TNT_SIDE = 15, TNT_TOP = 16, TNT_BOTTOM = 17, SNOW = 18, GRASS_BLOCK_SNOW = 19, OAK_LEAVES = 20, ICE = 21, CLAY = 22, SPRUCE_LOG = 23, SPRUCE_LOG_TOP = 24, OBSIDIAN = 25, SPONGE = 26, GLASS = 27,
	GOLD_ORE = 28, IRON_ORE = 29, COAL_ORE = 30, DIAMOND_ORE = 31, REDSTONE_ORE = 32, GRASS = 33, RED_MUSHROOM = 34, BROWN_MUSHROOM = 35, RED_TULIP = 36, DANDELION = 37, CRAFTING_TABLE_SIDE = 38, CRAFTING_TABLE_TOP = 39,
	CRAFTING_TABLE_FRONT = 40, STONE_BRICKS = 41, TALL_GRASS_TOP = 42, TALL_GRASS_BOTTOM = 43, FERN = 44, LARGE_FERN_TOP = 45, LARGE_FERN_BOTTOM = 46, LAPIS_BLOCK = 47;

	static GLuint atlas;
	static GLuint loadBMP_custom(const char * imagepath);
	static void loadTextures();
	static GLuint getTextureAtlas();
	static void generateTextureAtlas();
	static glm::vec2 toAtlasCoords(float uvx, float uvy, int id, int side);
};

#endif
