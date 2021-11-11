#ifndef BOUNDINGBOXRENDERER_H_
#define BOUNDINGBOXRENDERER_H_
class ShaderProgram;

class Game;

class BoundingBoxRenderer {
public:
	BoundingBoxRenderer();
	ShaderProgram *shader;
	void render(Game * game);
	virtual ~BoundingBoxRenderer();
};

#endif /* BOUNDINGBOXRENDERER_H_ */
