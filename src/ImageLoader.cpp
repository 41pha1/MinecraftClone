#include "ImageLoader.h"

#include <glm/detail/type_vec2.hpp>
#include <iostream>
#include <string>
#include <vector>

#include "Block.h"
#include "lodepng.h"

GLuint ImageLoader::atlas;

std::string blockTextures[] = {"grass_block_side", "grass_block_top", "dirt", "cobblestone", "stone", "oak_planks", "bricks", "bedrock",
		"sand", "gravel", "oak_log", "oak_log_top", "iron_block", "gold_block", "diamond_block", "tnt_side","tnt_top","tnt_bottom",
		"snow", "grass_block_snow", "oak_leaves", "ice", "clay", "spruce_log", "spruce_log_top", "obsidian", "sponge", "glass", "gold_ore", "iron_ore",
		"coal_ore", "diamond_ore", "redstone_ore",  "grass", "red_mushroom", "brown_mushroom", "poppy", "dandelion", "crafting_table_side",
		"crafting_table_top", "crafting_table_front", "stone_bricks", "tall_grass_top", "tall_grass_bottom", "fern", "large_fern_top", "large_fern_bottom", "lapis_block",
		"andesite", "diorite", "granite", "lapis_ore", "melon_side", "melon_top", "coarse_dirt", "chiseled_quartz_block", "dark_prismarine",
		"prismarine_bricks", "polished_diorite", "quartz_bricks", "sea_lantern", "cyan_concrete", "mossy_cobblestone", "cracked_stone_bricks", "quartz_pillar", "quartz_pillar_top",
		"quartz_block_side", "quartz_block_bottom", "quartz_block_top"};

void ImageLoader::loadTextures()
{
	generateTextureAtlas();
}

void ImageLoader::generateTextureAtlas()
{

	int amount = sizeof(blockTextures)/sizeof(blockTextures[0]);
	float atlasData[width*height*4];

	for(int x = 0; x < width*height*4; x++)
		atlasData[x] = 0;

	for(int i = 0; i < amount; i++)
	{
		std::vector<unsigned char> data;
		unsigned texWidth, texHeight;
		unsigned error = lodepng::decode(data, texWidth, texHeight, "res/textures/block/" + blockTextures[i] + ".png");
		if(error) std::cout << "decoder error " << error << ": " << lodepng_error_text(error) << std::endl;
		if(texRes != texWidth || texRes != texHeight)   std::cerr << "invalid texture resolution: " << blockTextures[i] + ".png" << std::endl;

		int rows = height/texRes;
		int columns = width/texRes;

		int xOff = (i % columns)*texRes;
		int yOff = (i / columns)*texRes;

		if(yOff/texRes >= (unsigned)rows)std::cerr << "Texture atlas too small!" << std::endl;

		int dataIndex = 0;
		for(unsigned y = 0; y < texHeight; y++)
		{
			for(unsigned x = 0; x < texWidth; x++)
			{
				for(int v = 0; v < 4; v++)
				{
					int ax = xOff + x;
					int ay = yOff + y;
					atlasData[(ax+ay*width)*4 + v] = data[dataIndex++]/255.;
				}
			}
		}
	}

	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_FLOAT, &atlasData[0]);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);

	atlas = textureID;
}

GLuint ImageLoader::loadBMP_custom(const char* imagepath)
{
	std::vector<unsigned char> data;
	unsigned width, height;
	unsigned error = lodepng::decode(data, width, height, imagepath);
	if(error) std::cout << "decoder error " << error << ": " << lodepng_error_text(error) << std::endl;

	int bytes = width*height*4;
	float * image = new float[bytes];

	for(int i = 0; i < bytes; i++)
		image[i] = data[i]/255.;

	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_FLOAT, &image[0]);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);

	return textureID;
}

GLuint ImageLoader::getTextureAtlas()
{
	return atlas;
}

glm::vec2 ImageLoader::toAtlasCoords(float uvx, float uvy,  int id, int side)
{
	glm::vec2 uv(uvx, uvy);
	int texID = 0;

	switch(id)
	{
	case Block::DIRT: texID = ImageLoader::DIRT; break;
	case Block::COBBLESTONE:  texID = ImageLoader::COBBLESTONE; break;
	case Block::GRASS_BLOCK:
		if(side == Block::TOP)
			texID = ImageLoader::GRASS_BLOCK_TOP;
		else if(side == Block::BOTTOM)
			texID = ImageLoader::DIRT;
		else
			texID = ImageLoader::GRASS_BLOCK_SIDE;
		break;
	case Block::STONE:  texID = ImageLoader::STONE; break;
	case Block::OAK_PLANK:  texID = ImageLoader::OAK_PLANKS; break;
	case Block::BRICK:  texID = ImageLoader::BRICKS; break;
	case Block::BEDROCK:  texID = ImageLoader::BEDROCK; break;
	case Block::SAND: texID = ImageLoader::SAND; break;
	case Block::GRAVEL:  texID = ImageLoader::GRAVEL; break;
	case Block::OAK_LOG:
		if(side == Block::TOP || side == Block::BOTTOM)
			texID = ImageLoader::OAK_LOG_TOP;
		else
			texID = ImageLoader::OAK_LOG;
		break;
	case Block::IRON_BLOCK:  texID = ImageLoader::IRON_BLOCK; break;
	case Block::GOLD_BLOCK: texID = ImageLoader::GOLD_BLOCK; break;
	case Block::DIAMOND_BLOCK:  texID = ImageLoader::DIAMOND_BLOCK; break;
	case Block::TNT:
		if(side == Block::TOP)
			texID = ImageLoader::TNT_TOP;
		else if(side == Block::BOTTOM)
			texID = ImageLoader::TNT_BOTTOM;
		else
			texID = ImageLoader::TNT_SIDE;
		break;
	case Block::SNOW:
		if(side == Block::TOP)
			texID = ImageLoader::SNOW;
		else if(side == Block::BOTTOM)
			texID = ImageLoader::DIRT;
		else
			texID = ImageLoader::GRASS_BLOCK_SNOW;
		break;
	case Block::LEAF: texID = ImageLoader::OAK_LEAVES; break;
	case Block::CLAY: texID = ImageLoader::CLAY; break;
	case Block::SPRUCE_LOG:
		if(side == Block::TOP || side == Block::BOTTOM)
			texID = ImageLoader::SPRUCE_LOG_TOP;
		else
			texID = ImageLoader::SPRUCE_LOG;
		break;
	case Block::OBSIDIAN:  texID = ImageLoader::OBSIDIAN; break;
	case Block::SPONGE:  texID = ImageLoader::SPONGE; break;
	case Block::GLASS:  texID = ImageLoader::GLASS; break;
	case Block::GOLD_ORE:  texID = ImageLoader::GOLD_ORE; break;
	case Block::IRON_ORE:  texID = ImageLoader::IRON_ORE; break;
	case Block::COAL_ORE:  texID = ImageLoader::COAL_ORE; break;
	case Block::DIAMOND_ORE:  texID = ImageLoader::DIAMOND_ORE; break;
	case Block::REDSTONE_ORE:  texID = ImageLoader::REDSTONE_ORE; break;
	case Block::GRASS:  texID = ImageLoader::GRASS; break;
	case Block::RED_MUSHROOM:  texID = ImageLoader::RED_MUSHROOM; break;
	case Block::BROWN_MUSHROOM:  texID = ImageLoader::BROWN_MUSHROOM; break;
	case Block::POPPY:  texID = ImageLoader::POPPY; break;
	case Block::DANDELION:  texID = ImageLoader::DANDELION; break;
	case Block::CRAFTING_BENCH:
		if(side == Block::TOP)
			texID = ImageLoader::CRAFTING_TABLE_TOP;
		else if(side == Block::BOTTOM)
			texID = ImageLoader::OAK_PLANKS;
		else if(side == Block::NORTH || side == Block::SOUTH)
			texID = ImageLoader::CRAFTING_TABLE_FRONT;
		else
			texID = ImageLoader::CRAFTING_TABLE_SIDE;
		break;
	case Block::STONE_BRICK:  texID = ImageLoader::STONE_BRICKS; break;
	case Block::SNOW_BLOCK:   texID = ImageLoader::SNOW; break;
	case Block::FERN:  texID = ImageLoader::FERN; break;
	case Block::LAPIS_LAZULI:  texID = ImageLoader::LAPIS_BLOCK; break;
	case Block::ICE: texID = ImageLoader::ICE; break;
	case Block::ANDESITE: texID = ImageLoader::ANDESITE; break;
	case Block::GRANITE: texID = ImageLoader::GRANITE; break;
	case Block::DIORITE: texID = ImageLoader::DIORITE; break;
	case Block::LAPIS_ORE: texID = ImageLoader::LAPIS_ORE; break;
	case Block::COARSE_DIRT: texID = ImageLoader::COARSE_DIRT; break;
	case Block::MELON:
		if(side == Block::TOP)
			texID = ImageLoader::MELON_TOP;
		else
			texID = ImageLoader::MELON_SIDE;
		break;
	case Block::CHISELED_QUARTZ_BLOCK: texID = ImageLoader::CHISELED_QUARTZ_BLOCK; break;
	case Block::DARK_PRISMARINE: texID = ImageLoader::DARK_PRISMARINE; break;
	case Block::PRISMARINE_BRICKS: texID = ImageLoader::PRISMARINE_BRICKS; break;
	case Block::POLISHED_DIORITE: texID = ImageLoader::POLISHED_DIORITE; break;
	case Block::QUARTZ_BRICKS: texID = ImageLoader::QUARTZ_BRICKS; break;
	case Block::SEA_LANTERN: texID = ImageLoader::SEA_LANTERN; break;
	case Block::CYAN_CONCRETE: texID = ImageLoader::CYAN_CONCRETE; break;
	case Block::MOSSY_COBBLESTONE: texID = ImageLoader::MOSSY_COBBLESTONE; break;
	case Block::CRACKED_STONE_BRICKS: texID = ImageLoader::CRACKED_STONE_BRICKS; break;
	case Block::QUARTZ_PILLAR:
		if(side == Block::TOP || side == Block::BOTTOM)
			texID = ImageLoader::QUARTZ_PILLAR_TOP;
		else
			texID = ImageLoader::QUARTZ_PILLAR;
		break;
	case Block::QUARTZ_BLOCK:
		if(side == Block::TOP)
			texID = ImageLoader::QUARTZ_BLOCK_TOP;
		else if(side == Block::BOTTOM)
			texID = ImageLoader::QUARTZ_BLOCK_BOTTOM;
		else
			texID = ImageLoader::QUARTZ_BLOCK_SIDE;
		break;
	}
	int rows = height / texRes;
	int columns = width / texRes;
	glm::vec2 offset = glm::vec2(texID % columns, texID/columns);
	return (uv  + offset) / glm::vec2(columns, rows);
}
