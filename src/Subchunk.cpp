#include "Subchunk.h"

#include <glm/ext/vector_float2.hpp>
#include <array>

#include "Block.h"
#include "BlockPalette.h"
#include "Chunk.h"
#include "ImageLoader.h"
#include "VAO.h"

Subchunk::Subchunk(Chunk * parent_, BlockPalette * blocks_, int cx_, int cy_, int cz_)
{
	parent = parent_;
	blocks = blocks_;
	cx = cx_;
	cy = cy_;
	cz = cz_;
	valid = true;
}

void Subchunk::setBlock(int x, int y, int z, int id)
{
	blocks->set(x,y,z,id);
}

char Subchunk::getBlock(int x, int y, int z)
{
	return blocks->get(x,y,z);
}

bool Subchunk::visibleOnSide(char id, int x, int y, int z)
{
	char other = getBlock(x, y, z);
	return Block::isTransparent(other) && (id != other || id == Block::LEAF);
}

int Subchunk::calculateBlockVisibility(int x, int y, int z)
{
	char id = parent->getBlock(x, y, z);
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

void Subchunk::generateMesh()
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

Subchunk::~Subchunk()
{
	delete blocks;
	delete vao;
}
