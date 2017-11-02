#pragma once
#include "OGLRenderer.h"

enum MeshBuffer {
	VERTEX_BUFFER, COLOUR_BUFFER, TEXTURE_BUFFER, MAX_BUFFER
};

class Mesh
{
public:
	Mesh();
	~Mesh();

	virtual void Draw();

	static Mesh* GenerateTriangle();

	static Mesh* GenerateQuad();

	static Mesh* GenerateQuadPatch();

	inline void SetTexture(GLuint texture) { this->texture = texture; }
	inline GLuint GetTexture() { return texture; }

protected:
	void BufferData();

	GLuint arrayObject;
	GLuint bufferObject[MAX_BUFFER];
	GLuint numVertices;
	GLuint type;

	Vector3*	vertices;
	Vector4*	colours;
	GLuint		texture;
	Vector2*	textureCoords;
};

