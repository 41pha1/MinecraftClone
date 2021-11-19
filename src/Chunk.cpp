#include "Chunk.h"

#include <glm/detail/type_mat4x4.hpp>
#include <glm/detail/type_vec3.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_float3.hpp>
#include <iostream>

#include "Block.h"
#include "Game.h"
#include "ImageLoader.h"
#include "VAO.h"
#include "VAOLoader.h"
#include "WorldGenerator.h"

class ImageLoader;

Chunk::Chunk(Game * game_, int cx_, int cz_)
{
	cx = cx_;
	cz = cz_;
	game = game_;
	valid = true;
	subchunks = new std::array<Subchunk*, HEIGHT>();

	game->generator->generateTerrain(this);
	game->generator->populateTerrain(this);

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(SIZE*cx,0,SIZE*cz));
	meshed = false;
}

Chunk::Chunk(Game * game_, std::array<Subchunk*, HEIGHT>* subchunks_, int cx_, int cz_)
{
	subchunks = subchunks_;

	cx = cx_;
	cz = cz_;
	game = game_;
	valid = true;
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(SIZE*cx,0,SIZE*cz));
	meshed = false;
}

void Chunk::setBlock(int x, int y, int z, int id)
{
	if(!valid)
		return;

	if(x < 0 || x >= SIZE || y < 0 || y >= SIZE * HEIGHT || z < 0 || z >= SIZE)
		return game->setBlock(cx * SIZE + x, y, cz * SIZE + z, id);

	int cy = y / SIZE;
	subchunks->operator [](cy)->setBlock(x,y % SIZE,z,id);
}

char Chunk::getBlock(int x, int y, int z)
{
	if(!valid)
		return Block::UNKNOWN;

	if( y < 0 || y >= SIZE * HEIGHT)
		return Block::AIR;

	if(x < 0 || x >= SIZE || z < 0 || z >= SIZE)
		return game->getBlock(cx * SIZE + x, y, cz * SIZE + z);

	int cy = y / SIZE;
	return subchunks->operator [](cy)->getBlock(x,y % SIZE,z);
}

bool Chunk::visibleOnSide(char id, int x, int y, int z)
{
	char other = getBlock(x, y, z);
	return Block::isTransparent(other) && (id != other || id == Block::LEAF);
}

int Chunk::calculateBlockVisibility(int x, int y, int z, char id)
{
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

	auto visibilities = std::array<int, SIZE*SIZE*SIZE*HEIGHT>();

	int i = 0;
	vCount = 0;
	iCount = 0;

	for(int x = 0; x < SIZE; x++)
		for(int y = 0; y < SIZE * HEIGHT; y++)
			for(int z = 0; z < SIZE; z++)
			{
				char block = getBlock(x,y,z);

				if(!Block::isRenderable(block))
					visibilities[i] = 0;
				else
					visibilities[i] = calculateBlockVisibility(x, y, z, block);

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
		for(int y = 0; y < SIZE*HEIGHT; y++)
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
	meshed = true;
}

void Chunk::loadToVao()
{
	if(vao != 0)
		delete vao;

	vao = VAOLoader::loadToVAO(verts, normals, uvs, indices, vCount, (vCount*2)/3, iCount);

	delete[] verts;
	delete[] normals;
	delete[] uvs;
	delete[] indices;
}

Chunk::Chunk()
{
	valid = false;
	cx = 0;
	cz = 0;
	subchunks = 0;
	game = 0;
	meshed = false;
}

Chunk::~Chunk()
{
	for(int i = 0; i < HEIGHT; i++)
		delete subchunks->operator [](i);
	delete subchunks;
	delete vao;
}
