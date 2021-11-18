#include "Chunk.h"

#include <glm/detail/type_mat4x4.hpp>
#include <glm/detail/type_vec3.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_float3.hpp>
#include <stdlib.h>
#include <array>
#include <iostream>

#include "Block.h"
#include "Game.h"
#include "ImageLoader.h"
#include "VAOLoader.h"
#include "WorldGenerator.h"

class ImageLoader;

Chunk::Chunk(Game * game_, int SEED, int cx_, int cy_, int cz_, bool generate)
{
	cx = cx_;
	cy = cy_;
	cz = cz_;
	game = game_;
	valid = false;

//	blocks = new char**[SIZE];
//	for(int x = 0; x < SIZE; x++)
//	{
//		blocks[x] = new char*[SIZE];
//		for(int y = 0; y < SIZE; y++)
//		{
//			blocks[x][y] = new char[SIZE];
//		}
//	}
//	blocks = new Octree(SIZE);
	blocks = new BlockPalette(SIZE);

	if(generate)
	{
		game->generator->generateTerrain(this);
		game->generator->populateTerrain(this);
		valid = true;
	}

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(SIZE*cx,SIZE*cy,SIZE*cz));
}

Chunk::Chunk(Game * game_, char*** blocks_, int cx_, int cy_, int cz_)
{
	blocks = new BlockPalette(SIZE);
//	blocks = new Octree(SIZE);

	if(blocks_ == 0)
		blocks->set(0,0,0,0);
	else
		for(int x = 0; x < SIZE; x++)
			for(int y = 0; y < SIZE; y++)
				for(int z = 0; z < SIZE; z++)
					blocks->set(x,y,z, blocks_[x][y][z]);

//	blocks = blocks_;
	cx = cx_;
	cy = cy_;
	cz = cz_;
	game = game_;
	valid = true;
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(SIZE*cx,SIZE*cy,SIZE*cz));
}

void Chunk::setBlock(int x, int y, int z, int id)
{
	if(x < 0 || x >= SIZE || y < 0 || y >= SIZE || z < 0 || z >= SIZE)
			return game->setBlock(cx * SIZE + x, cy * SIZE + y, cz * SIZE + z, id);

//	blocks[x][y][z] = id;
	blocks->set(x,y,z,id);
}

char Chunk::getBlock(int x, int y, int z)
{
	if(x < 0 || x >= SIZE || y < 0 || y >= SIZE || z < 0 || z >= SIZE)
			return game->getBlock(cx * SIZE + x, cy * SIZE + y, cz * SIZE + z);

//	if(blocks == 0)
//		return Block::AIR;

//	return blocks[x][y][z];
	return blocks->get(x,y,z);
//	return blocks.get(y*SIZE*SIZE+ x*SIZE + z);
}

bool Chunk::visibleOnSide(char id, int x, int y, int z)
{
	char other = getBlock(x, y, z);
	return Block::isTransparent(other) && (id != other || id == Block::LEAF);
}

int Chunk::calculateBlockVisibility(int x, int y, int z)
{
	char id = getBlock(x, y, z);
	int visibility = 0;
	visibility += visibleOnSide(id, x+1, y, z);
	visibility <<= 1;
	visibility += visibleOnSide(id, x-1, y, z);
	visibility <<= 1;
	visibility +=  visibleOnSide(id, x, y, z+1);
	visibility <<= 1;
	visibility +=  visibleOnSide(id, x, y, z-1);
	visibility <<= 1;
	visibility +=  visibleOnSide(id, x, y+1, z);
	visibility <<= 1;
	visibility += visibleOnSide(id, x, y-1, z);
	return visibility;
}

void Chunk::generateMesh()
{
	if(!valid)
		return;

	auto visibilities = std::array<int, SIZE*SIZE*SIZE>();

	int i = 0;
	vCount = 0;
	iCount = 0;

	for(int x = 0; x < SIZE; x++)
		for(int y = 0; y < SIZE; y++)
			for(int z = 0; z < SIZE; z++)
			{
				char block = getBlock(x,y,z);

				if(!Block::isRenderable(block))
					visibilities[i] = 0;
				else
					visibilities[i] = calculateBlockVisibility(x, y, z);


				vCount += Block::getVertexCount(block, visibilities[i]);
				iCount += Block::getIndexCount(block, visibilities[i++]);
			}

	verts = new GLfloat[vCount];
	normals = new GLfloat[vCount];
	uvs = new GLfloat[(vCount*2)/3];
	indices = new GLuint[iCount];

	int v = 0, n = 0, u = 0, in = 0;

	i = 0;
	int index = 0;
	for(int x = 0; x < SIZE; x++)
		for(int y = 0; y < SIZE; y++)
			for(int z = 0; z < SIZE; z++)
			{
				int visibility = visibilities[i++];
				if(!visibility)
					continue;
				char block = getBlock(x,y,z);

				int vertCount = Block::getVertexCount(block, visibility);
				int uvCount = (vertCount*2)/3;
				int indexCount = Block::getIndexCount(block, visibility);

				bool blockMesh =	Block::getMeshType(block) == Block::BLOCK;

				const float * blockVertices =Block::getVertices(block, visibility);
				const float * blockUVs = Block::getUVs(block, visibility);
				const float * blockNormals = Block::getNormals(block, visibility);
				const int * blockIndices = Block::getIndices(block, visibility);

				for(int i = 0; i < vertCount; i++)
				{
					normals[n++] = blockNormals[i];
				}


				for(int i = 0; i < indexCount; i++)
				{
					indices[in++] = blockIndices[i]+index;
				}
				index+=vertCount/3;

				for(int i = 0; i < vertCount; i+=3)
				{
					verts[v++] = blockVertices[i]+x;
					verts[v++] = blockVertices[i+1]+y;
					verts[v++] = blockVertices[i+2]+z;
				}
				int side = -1;
				int j = 32;
				for(int i = 0; i < uvCount; i+=2)
				{
					if(blockMesh && !(i % 8))
					{
						side++;
						while(!(j & visibility))
						{
							j = j >> 1;
							side++;
						}
						j = j >> 1;
					}

					glm::vec2 atlasuv = ImageLoader::toAtlasCoords(blockUVs[i], blockUVs[i+1], block, side);
					uvs[u++] = atlasuv.x;
					uvs[u++] = atlasuv.y;
				}
			}
}

void Chunk::loadToVao()
{
//	vao = VAOLoader::loadToVAO(verts, normals, uvs, indices, vCount, (vCount*2)/3, iCount);

	delete[] verts;
	delete[] normals;
	delete[] uvs;
	delete[] indices;
}

Chunk::~Chunk()
{
//	for(int x = 0; x < SIZE; x++)
//		for(int y = 0; y < SIZE; y++)
//			delete[] blocks[x][y];
//
//	for(int x = 0; x < SIZE; x++)
//		delete[] blocks[x];
//
//	delete[] blocks;
	delete blocks;
	delete vao;
}

Chunk::Chunk()
{
//	blocks = new Octree(SIZE);
//	blocks = new char**[SIZE];
	blocks = new BlockPalette(SIZE);
	valid = false;
	cx = 0;
	cy = 0;
	cz = 0;
	game = 0;
}
