#pragma once
#include"Shader.h"

#define POST_PASSES 10

class PostProcessingEffect
{
public:
	PostProcessingEffect(int width, int height, Shader* sceneShader, Shader* processingShader);
	~PostProcessingEffect();

	inline static void GenerateQuad() {
		quad = Mesh::GenerateQuad();
	}

	inline static void DeleteQuad() {
		if (quad) {
			delete quad;
		}
		
	}

	inline GLuint GetFrameBuffer() {
		return bufferFBO;
	}

	virtual void Draw();

	virtual void Present();

	void UpdateShaderMatrices(Shader* shader);

protected:
	Shader* sceneShader;
	Shader* processShader;
	GLuint bufferFBO;
	GLuint processFBO;
	GLuint bufferColourTex[2];
	GLuint bufferDepthTex;

	Matrix4 modelMatrix;
	Matrix4 projMatrix;
	Matrix4 viewMatrix;

	int width;
	int height;

	static Mesh* quad;
};

