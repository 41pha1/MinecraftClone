#ifndef CHUNK_H_
#define CHUNK_H_

#include <glad/glad.h>
#include <glm/ext/matrix_float4x4.hpp>
#include <vector>

#include "Block.h"
#include "VAO.h"
#include "Octree.h"

class Chunk
{
public:
	static const int SIZE = 16;
	int cx, cy, cz;
	bool valid;
//	char *** blocks = new char**[0];
	Octree * blocks;
	Game * game;
	VAO * vao = 0;
	glm::mat4 model;

	Chunk();
	Chunk(Game * game, int SEED, int cx, int cy, int cz);
	Chunk(Game * game, char *** blocks, int cx, int cy, int cz);
	char getBlock(int x, int y, int z);
	void setBlock(int x, int y, int z, int id);
	std::vector<GLfloat> createBlockMesh(int visibility, int x, int y, int z);
	std::vector<GLfloat> createBlockUVs(int visibility, int x, int y, int z);
	std::vector<GLfloat> createBlockNormals(int visibility, int x, int y, int z);
	bool visibleOnSide(char id, int x, int y, int z);
	int calculateBlockVisibility(int x, int y, int z);
	void generateMesh();


	virtual ~Chunk();
};

#endif
