#pragma once
#include "../nclgl/OGLRenderer.h"
class Texture
{
public:
	Texture(string filePath);
	~Texture();

	inline GLuint*	GetTexture() { return &texture; }
	inline void		SetTexture(GLuint texture) { this->texture = texture; }

	inline Matrix4 GetTextureMatrix() { return textureMatrix; }
	inline void  SetTextureMatrix(Matrix4 textureMatrix) { this->textureMatrix = textureMatrix; }

	void ToggleRepeating();

	void ToggleFiltering();

	void RotateMatrix(float angle);

protected:
	GLuint	texture;
	Matrix4 textureMatrix;

	bool repeating;
	bool filtering;
};

