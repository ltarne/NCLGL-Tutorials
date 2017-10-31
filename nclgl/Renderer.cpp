#include "Renderer.h"

Renderer::Renderer(Window &parent) : OGLRenderer(parent)	{
	camera = new Camera();

	currentShader = new Shader(SHADERDIR"basicVert.vert",
		SHADERDIR"basicFrag.frag");

	if(!currentShader->LinkProgram()) {
		return;
	}

	projMatrix = Matrix4::Perspective(1.0f, 10000.0f, (float)width / (float)height, 45.0f);

	camera->SetPosition(Vector3(0, 0, 10));

	root = new SceneNode();
	root->AddChild(new SceneNode(Mesh::GenerateTriangle()));

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	init = true;
}
Renderer::~Renderer(void)	{
	delete root;
}

void Renderer::UpdateScene(float msec) {
	camera->UpdateCamera(msec);
	viewMatrix = camera->BuildViewMatrix();
	root->Update(msec);
}

void Renderer::RenderScene()	{
	glClearColor(0.2f,0.2f,0.2f,1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_COLOR_BUFFER_BIT);	

	glUseProgram(currentShader->GetProgram());
	UpdateShaderMatrices();

	DrawNode(root);
	glUseProgram(0);

	SwapBuffers();	
}

void Renderer::SwitchToPerspective() {
	projMatrix = Matrix4::Perspective(1.0f, 10000.0f, (float)width / (float)height, 45.0f);
}

void Renderer::SwitchToOrthographic() {
	projMatrix = Matrix4::Orthographic(-1.0f, 10000.0f, width / 2.0f, -width / 2.0f, height / 2.0f, -height / 2.0f);
}

void Renderer::DrawNode(SceneNode* node) {
	if (node->GetMesh()) {
		Matrix4 transform = node->GetWorldTransform() * Matrix4::Scale(node->GetModelScale());

		node->Draw(*this);
	}

	for (vector<SceneNode*>::const_iterator i = node->GetChildIteratorStart(); i != node->GetChildIteratorEnd(); ++i) {
		DrawNode(*i);
	}
}

