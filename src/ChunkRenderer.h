#ifndef CHUNKRENDERER_H_
#define CHUNKRENDERER_H_
class Camera;
class Chunk;
class ShaderProgram;

class ChunkRenderer {
public:
	ChunkRenderer();
	virtual ~ChunkRenderer();
	void render(ShaderProgram* shader, Chunk* chunk, Camera* cam);
};

#endif
