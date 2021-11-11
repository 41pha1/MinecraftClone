#include "BlockShader.h"

BlockShader::BlockShader()
{
	ProgramID = LoadShaders(vertexShaderFile, fragmentShaderFile);
}

BlockShader::~BlockShader()
{

}

