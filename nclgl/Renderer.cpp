#include "Renderer.h"

Renderer::Renderer(Window &parent) : OGLRenderer(parent)	{
	camera = new Camera();

	projMatrix = Matrix4::Perspective(1.0f, 10000.0f, (float)width / (float)height, 45.0f);

	camera->SetPosition(Vector3(0, 0, 2));

	root = new SceneNode(debugDrawShader);

	glEnable(GL_DEPTH_TEST);
	//glDisable(GL_CULL_FACE);

	init = true;

	usingDepth = true;
	usingAlpha = false;
	blendMode = 0;
	usingScissor = false;
	usingStencil = false;
}
Renderer::~Renderer(void)	{
	delete root;
	delete camera;
}

void Renderer::UpdateScene(float msec) {
	camera->UpdateCamera(msec);
	viewMatrix = camera->BuildViewMatrix();
	frameFrustrum.FromMatrix(projMatrix * viewMatrix);

	root->Update(msec);
}

void Renderer::RenderScene()	{
	BuildNodeLists(root);
	SortNodeLists();

	glClearColor(0.2f,0.2f,0.2f,1.0f);
	glScissor(0, 0, width, height);//Sets the scissor region to the whole screen before the clear
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);	

	if (usingScissor) {
		
		glScissor((float)width / 2.5f, (float)height / 2.5f,
			(float)width / 5.0f, (float)height / 5.0f);
	}


	DrawNodes();

	
	SwapBuffers();	
	ClearNodeLists();
}

void Renderer::SwitchToPerspective() {
	projMatrix = Matrix4::Perspective(1.0f, 10000.0f, (float)width / (float)height, 45.0f);
}

void Renderer::SwitchToOrthographic() {
	projMatrix = Matrix4::Orthographic(-1.0f, 10000.0f, width / 2.0f, -width / 2.0f, height / 2.0f, -height / 2.0f);
}

void Renderer::ToggleAlphaBlend() {
	usingAlpha = !usingAlpha;
	usingAlpha ? glEnable(GL_BLEND) : glDisable(GL_BLEND);
}

void Renderer::ToggleBlendMode() {
	blendMode = (blendMode + 1) % 4;

	switch (blendMode) {
	case(0):
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		break;

	case(1):
		glBlendFunc(GL_SRC_COLOR, GL_ONE_MINUS_SRC_COLOR);
		break;

	case(2):
		glBlendFunc(GL_ONE, GL_ZERO);
		break;

	case(3):
		glBlendFunc(GL_SRC_ALPHA, GL_ALPHA);
		break;
	}
}

void Renderer::ToggleScissor() {
	usingScissor = !usingScissor;
	usingScissor ? glEnable(GL_SCISSOR_TEST) : glDisable(GL_SCISSOR_TEST);

}

void Renderer::ToggleStencil() {
	usingStencil = !usingStencil;
	usingStencil ? glEnable(GL_STENCIL_TEST) : glDisable(GL_STENCIL_TEST);
}

void Renderer::ToggleDepth() {
	usingDepth = !usingDepth;
	usingDepth ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
}

void Renderer::BuildNodeLists(SceneNode * from) {
	if (frameFrustrum.InsideFrustrum(*from)) {
		Vector3 dir = from->GetWorldTransform().GetPositionVector() - camera->GetPosition();

		from->SetCameraDistance(Vector3::Dot(dir, dir));

		if (from->GetColour().w < 1.0f) {
			transparentNodeList.push_back(from);
		}
		else {
			nodeList.push_back(from);
		}
	}

	for (vector<SceneNode*>::const_iterator i = from->GetChildIteratorStart(); i != from->GetChildIteratorEnd(); ++i) {
		BuildNodeLists((*i));
	}
}

void Renderer::SortNodeLists() {
	sort(transparentNodeList.begin(), transparentNodeList.end(), SceneNode::CompareByCameraDistance);

	sort(nodeList.begin(), nodeList.end(), SceneNode::CompareByCameraDistance);
}

void Renderer::ClearNodeLists() {
	transparentNodeList.clear();
	nodeList.clear();
}

void Renderer::DrawNodes() {
	for (vector<SceneNode*>::const_iterator i = nodeList.begin(); i != nodeList.end(); ++i) {
		DrawNode((*i));
	}

	for (vector<SceneNode*>::const_reverse_iterator i = transparentNodeList.rbegin(); i != transparentNodeList.rend(); ++i) {
		DrawNode((*i));
	}
}

void Renderer::DrawNode(SceneNode* node) {
	

	if (node->GetVisible() && node->GetMesh()) {
		glUseProgram(node->GetShader()->GetProgram());
		UpdateShaderMatrices(node->GetShader());

		node->Draw(*this);

		glUseProgram(0);
	}

	

	/*for (vector<SceneNode*>::const_iterator i = node->GetChildIteratorStart(); i != node->GetChildIteratorEnd(); ++i) {
		DrawNode(*i);
	}*/
}

