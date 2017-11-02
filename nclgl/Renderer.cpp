#include "Renderer.h"

Renderer::Renderer(Window &parent) : OGLRenderer(parent)	{
	camera = new Camera();

	projMatrix = Matrix4::Perspective(1.0f, 10000.0f, (float)width / (float)height, 45.0f);

	camera->SetPosition(Vector3(0, 0, 2));

	root = new SceneNode(debugDrawShader);


	//glDisable(GL_CULL_FACE);

	init = true;

	usingDepth = false;
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

	root->Update(msec);
}

void Renderer::RenderScene()	{
	glClearColor(0.2f,0.2f,0.2f,1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);	

	if (usingScissor) {
		
		glScissor((float)width / 2.5f, (float)height / 2.5f,
			(float)width / 5.0f, (float)height / 5.0f);
	}

	/*if (usingStencil) {
		

		glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
		glStencilFunc(GL_ALWAYS, 2, ~0);
		glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);
	}*/

	DrawNode(root);

	/*if (usingStencil) {
		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
		glStencilFunc(GL_EQUAL, 2, ~0);
		glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
	}*/
	
	SwapBuffers();	
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

void Renderer::DrawNode(SceneNode* node) {
	glUseProgram(node->GetShader()->GetProgram());
	UpdateShaderMatrices(node->GetShader());

	if (usingStencil) {
		node->ActivateSpecialSettings();
	}

	if (node->GetMesh()) {
		node->Draw(*this);
	}

	if (usingStencil) {
		node->DeactivateSpecialSettings();
	}

	glUseProgram(0);

	for (vector<SceneNode*>::const_iterator i = node->GetChildIteratorStart(); i != node->GetChildIteratorEnd(); ++i) {
		//Might be useful at some point to have a function to change opengl settings based on a nodes particular needs
		DrawNode(*i);
		//Which could then be reverted in another function
	}
}

