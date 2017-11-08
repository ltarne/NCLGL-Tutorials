#pragma once
#include "OGLRenderer.h"
#include "StencilNode.h"
#include "Camera.h"
#include "Frustum.h"
#include "MD5Node.h"
#include "PostProcessingEffect.h"
#include <algorithm>

#define POST_PASSES 10

class Renderer : public OGLRenderer	{
public:
	Renderer(Window &parent);
	virtual ~Renderer(void);

	virtual void UpdateScene(float msec);
	virtual void RenderScene();
	void RenderPPScene();

	//inline void SetProcessShader(Shader* processShader) { this->processShader = processShader; }

	void SwitchToPerspective();
	void SwitchToOrthographic();

	void ToggleAlphaBlend();
	void ToggleBlendMode();

	void ToggleScissor();
	void ToggleStencil();

	void AttachSceneGraph(SceneNode* node) { root->AddChild(node); }

	void ToggleDepth();
	
	void SetRotation(float rotation) { root->SetRotation(Matrix4::Rotation(rotation, Vector3(0,0,1))); }
	void SetScale(float scale) { root->SetScale(Matrix4::Scale(Vector3(scale,scale,scale))); }
	void SetPosition(Vector3 position) { root->SetTransform(Matrix4::Translation(position)); }

	


protected:
	/*void LoadPostProcessing();*/
	void BuildNodeLists(SceneNode* from);
	void SortNodeLists();
	void ClearNodeLists();
	void DrawNodes();
	void DrawNode(SceneNode* node);

	/*void PresentScene();
	void DrawPostProcess();*/
	void DrawScene();

	/*Shader* processShader;
	GLuint bufferFBO;
	GLuint processFBO;
	GLuint bufferColourTex[2];
	GLuint bufferDepthTex;*/

	SceneNode* root;
	/*SceneNode* quad;*/
	Camera* camera;
	Frustum frameFrustrum;
	PostProcessingEffect* blur;

	bool usingDepth;
	bool usingAlpha;
	int blendMode;
	bool usingScissor;
	bool usingStencil;


	vector<SceneNode*> transparentNodeList;
	vector<SceneNode*> nodeList;
};
