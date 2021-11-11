#include "Block.h"

#include "Game.h"

float ** Block::vertices;
float ** Block::normals;
float ** Block::uvs;

constexpr const float Block::foliageVertices[];
constexpr const float Block::foliageNormals[];
constexpr const float Block::foliageUVs[];
constexpr const int Block::foliageIndices[];
constexpr const int Block::indices[];

void Block::generateMeshData()
{
	//0,1,2,0,3,1,
	float northFace[] =  {
			1.0f, .0f, .0f,    //+
			1.0f, 1.0f, 1.0f,  //-
			1.0f, 0.0f, 1.0f,
			1.0f, 1.0f, 0.0f,
	};

	float southFace[] =  {
			0.0f, 1.0f, 1.0f, //+
			0.0f, 0.0f, 0.0f, //-
			0.0f, 0.0f, 1.0f,
			0.0f, 1.0f, 0.0f,
			};

	float eastFace[] =   {
			0.0f, 0.0f, 1.0f, //+
			1.0f, 1.0f, 1.0f, //-
			0.0f, 1.0f, 1.0f,
			1.0f, 0.0f, 1.0f,
	};
	float westFace[] =   {
			1.0f, 1.0f, 0.0f,  //+
			0.0f, 0.0f, 0.0f,  //-
			0.0f, 1.0f, 0.0f,
			1.0f, 0.0f, 0.0f,
			};

	float topFace[] =    {
			1.0f, 1.0f, 1.0f, //+
			0.0f, 1.0f, 0.0f, //-
			0.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 0.0f,
	};
	float bottomFace[] = {
			0.0f, 0.0f, 0.0f,  //+
			1.0f, 0.0f, 1.0f,  //-
			0.0f, 0.0f, 1.0f,
			1.0f, 0.0f, 0.0f,
	};

	float northNormals[] =  {1.0f, .0f, .0f, 1.0f, .0f, .0f,1.0f, .0f, .0f,1.0f, .0f, .0f};
	float southNormals[] =  {-1.0f, .0f, .0f,-1.0f, .0f, .0f,-1.0f, .0f, .0f,-1.0f, .0f, .0f};
	float eastNormals[] =   {0.0f, .0f, 1.0f,0.0f, .0f, 1.0f,0.0f, .0f, 1.0f,0.0f, .0f, 1.0f};
	float westNormals[] =   {0.0f, .0f, -1.0f,0.0f, .0f, -1.0f,0.0f, .0f, -1.0f,0.0f, .0f, -1.0f};
	float topNormals[] =    {0.0f, 1.0f, 0.0f,0.0f, 1.0f, 0.0f,0.0f, 1.0f, 0.0f,0.0f, 1.0f, 0.0f};
	float bottomNormals[] = {0.0f, -1.0f, 0.0f,0.0f, -1.0f, 0.0f,0.0f, -1.0f, 0.0f,0.0f, -1.0f, 0.0f};

	float northUVs[] =  {1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f};
	float southUVs[] =  {1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f};
	float eastUVs[] =   {0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f};
	float westUVs[] =   {0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f};
	float topUVs[] =    {1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f};
	float bottomUVs[] = {0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f};

	vertices = new float*[64];
	normals = new float*[64];
	uvs = new float*[64];

	for(int v = 0; v < 64; v++)
	{
		int size = __builtin_popcount(v) * 12;
		vertices[v] = new float[size];
		normals[v] = new float[size];
		uvs[v] = new float[__builtin_popcount(v)*8];

		int uv_index = 0;
		int index = 0;
		if(v & 32)
			for(int i = 0; i < 12; i++){
				normals[v][index] = northNormals[i];
				vertices[v][index++] = northFace[i];
				if(i < 8)
					uvs[v][uv_index++] = northUVs[i];
			}
		if(v & 16)
			for(int i = 0; i < 12; i++){
				normals[v][index] = southNormals[i];
				vertices[v][index++] = southFace[i];
				if(i < 8)
					uvs[v][uv_index++] = southUVs[i];
			}
		if(v & 8)
			for(int i = 0; i < 12; i++){
				normals[v][index] = eastNormals[i];
				vertices[v][index++] = eastFace[i];
				if(i < 8)
					uvs[v][uv_index++] = eastUVs[i];
			}
		if(v & 4)
			for(int i = 0; i < 12; i++){
				normals[v][index] = westNormals[i];
				vertices[v][index++] = westFace[i];
				if(i < 8)
					uvs[v][uv_index++] = westUVs[i];
			}
		if(v & 2)
			for(int i = 0; i < 12; i++){
				normals[v][index] = topNormals[i];
				vertices[v][index++] = topFace[i];
				if(i < 8)
					uvs[v][uv_index++] = topUVs[i];
			}
		if(v & 1)
			for(int i = 0; i < 12; i++){
				normals[v][index] = bottomNormals[i];
				vertices[v][index++] =bottomFace[i];
				if(i < 8)
					uvs[v][uv_index++] = bottomUVs[i];
		}
	}
}


Block::Block()
{

}

const float* Block::getVertices(char id, int visibility)
{
	switch(getMeshType(id))
	{
	case FOLIAGE: return foliageVertices;
	}
	return vertices[visibility];
}

const float* Block::getUVs(char id, int visibility)
{
	switch(getMeshType(id))
	{
	case FOLIAGE: return foliageUVs;
	}
	return uvs[visibility];
}

const float* Block::getNormals(char id, int visibility)
{
	switch(getMeshType(id))
	{
	case FOLIAGE: return foliageNormals;
	}
	return normals[visibility];
}

const int* Block::getIndices(char id, int visibility)
{
	switch(getMeshType(id))
	{
	case FOLIAGE: return foliageIndices;
	}
	return indices;
}

bool Block::isTransparent(char id)
{
	switch(id)
	{
	case AIR: case UNKNOWN: case LEAF: case BROWN_MUSHROOM: case GLASS: case RED_FLOWER: case RED_MUSHROOM: case YELLOW_FLOWER: case GRASS: case FERN: return true;
	default: return false;
	}
}

bool Block::isRenderable(char id)
{
	switch(id)
	{
	case AIR: case UNKNOWN: return false;
	default: return true;
	}
}

bool Block::hasBlockCollision(char id)
{
	switch(id)
	{
	case AIR: case UNKNOWN: case BROWN_MUSHROOM: case RED_FLOWER: case RED_MUSHROOM: case YELLOW_FLOWER: case GRASS: case FERN: return false;
	default: return true;
	}
}

int Block::getMeshType(char id)
{
	switch(id)
	{
	case RED_FLOWER: case YELLOW_FLOWER: case GRASS: case RED_MUSHROOM: case BROWN_MUSHROOM: case FERN: return FOLIAGE;
	default: return BLOCK;
	}
}

int Block::getVertexCount(char id, int v)
{
	switch(getMeshType(id))
	{
	case FOLIAGE: return 48;
	}
	return __builtin_popcount(v)*12;
}


int Block::getIndexCount(char id, int v)
{
	switch(getMeshType(id))
	{
	case FOLIAGE: return 24;
	}
	return __builtin_popcount(v)*6;
}

void Block::update(char id, Game* game, int x, int y, int z)
{
	switch(getMeshType(id))
	{
	case FOLIAGE:
		char groundID = game->getBlock(x, y-1, z);
		if(groundID != DIRT && groundID != GRASS_BLOCK)
			destroy(id, game, x, y, z);
	}
}

void Block::destroy(char id, Game* game, int x, int y, int z)
{
	game->placeBlock(x, y, z, AIR);
}