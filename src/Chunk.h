#ifndef CHUNK_H_
#define CHUNK_H_

#include <glad/glad.h>
#include <glm/ext/matrix_float4x4.hpp>
#include <array>

#include "Subchunk.h"

class Game;

class Chunk
{
public:
	static const int SIZE = 16, HEIGHT = 16;
	int cx, cz;
	bool valid, meshed;
	std::array<Subchunk*, HEIGHT>* subchunks;
	Game * game;
	VAO * vao = 0;
	glm::mat4 model;

	GLfloat * verts = 0;
	GLfloat * normals = 0;
	GLfloat * uvs = 0;
	GLuint * indices = 0;
	int vCount = 0, iCount = 0;

	Chunk();
	Chunk(Game * game, std::array<Subchunk*, HEIGHT>*, int cx, int cz);
	Chunk(Game * game, int cx, int cz);
	char getBlock(int x, int y, int z);
	void setBlock(int x, int y, int z, int id);
	void loadToVao();
	void generateMesh();
	bool visibleOnSide(char id, int x, int y, int z);
	int calculateBlockVisibility(int x, int y, int z, char id);

	virtual ~Chunk();
};

#endif
