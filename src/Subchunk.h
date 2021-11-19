#ifndef SUBCHUNK_H_
#define SUBCHUNK_H_

#include <glad/glad.h>
#include <vector>

class BlockPalette;
class Chunk;
class VAO;

class Subchunk
{
public:
	static const int SIZE = 16, HEIGHT = 16;
	int cx, cy, cz;
	bool valid;
	BlockPalette * blocks;
	VAO * vao = 0;
	Chunk * parent;

	GLfloat * verts = 0;
	GLfloat * normals = 0;
	GLfloat * uvs = 0;
	GLuint * indices = 0;
	int vCount = 0, iCount = 0;

	Subchunk(Chunk * parent, BlockPalette * blocks, int cx, int cy, int cz);
	char getBlock(int x, int y, int z);
	void setBlock(int x, int y, int z, int id);
	std::vector<GLfloat> createBlockMesh(int visibility, int x, int y, int z);
	std::vector<GLfloat> createBlockUVs(int visibility, int x, int y, int z);
	std::vector<GLfloat> createBlockNormals(int visibility, int x, int y, int z);
	bool visibleOnSide(char id, int x, int y, int z);
	int calculateBlockVisibility(int x, int y, int z);
	void generateMesh();
	virtual ~Subchunk();
};


#endif
