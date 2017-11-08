#include "Renderer.h"

Renderer::Renderer(Window &parent) : OGLRenderer(parent)	{
	camera = new Camera();

	projMatrix = Matrix4::Perspective(1.0f, 10000.0f, (float)width / (float)height, 45.0f);

	camera->SetPosition(Vector3(0, 0, 2));

	root = new SceneNode(debugDrawShader);

	glEnable(GL_DEPTH_TEST);
	//glDisable(GL_CULL_FACE);

	Shader* processShader = new Shader(SHADERDIR"TexturedVertex.glsl", SHADERDIR"processFrag.frag");
	processShader->LinkProgram();
	Shader* sceneShader = new Shader(SHADERDIR"TexturedVertex.glsl", SHADERDIR"TexturedFrag.glsl");
	sceneShader->LinkProgram();

	/*quad = new SceneNode(processShader, Mesh::GenerateQuad());*/
	PostProcessingEffect::GenerateQuad();
	blur = new PostProcessingEffect(width, height, sceneShader, processShader);
	
	/*LoadPostProcessing();*/

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
	PostProcessingEffect::DeleteQuad();

	/*glDeleteTextures(2, bufferColourTex);
	glDeleteTextures(1, &bufferDepthTex);
	glDeleteFramebuffers(1, &bufferFBO);
	glDeleteFramebuffers(1, &processFBO);*/
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

void Renderer::RenderPPScene() {
	DrawScene();
	blur->Draw();
	blur->Present();
	/*DrawPostProcess();
	PresentScene();*/
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

//void Renderer::LoadPostProcessing() {
//	glGenTextures(1, &bufferDepthTex);
//	glBindTexture(GL_TEXTURE_2D, bufferDepthTex);
//	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
//	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
//	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
//
//	for (int i = 0; i < 2; ++i) {
//		glGenTextures(1, &bufferColourTex[i]);
//		glBindTexture(GL_TEXTURE_2D, bufferColourTex[i]);
//		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
//		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
//		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
//	}
//
//	glGenFramebuffers(1, &bufferFBO); //Render scene into this
//	glGenFramebuffers(1, &processFBO); //Post processing in this
//
//	glBindFramebuffer(GL_FRAMEBUFFER, bufferFBO);
//	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, bufferDepthTex, 0);
//	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, bufferDepthTex, 0);
//	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, bufferColourTex[0], 0);
//
//	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE || !bufferDepthTex || !bufferColourTex[0]) {
//		cout << "Framebuffer failed!\n";
//		return;
//	}
//	glBindFramebuffer(GL_FRAMEBUFFER, 0);
//}

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

//void Renderer::PresentScene() {
//	glBindFramebuffer(GL_FRAMEBUFFER, 0);
//	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
//	
//}
//
//void Renderer::DrawPostProcess() {
//	glBindFramebuffer(GL_FRAMEBUFFER, processFBO);
//	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, bufferColourTex[1], 0);
//	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
//
//	projMatrix = Matrix4::Orthographic(-1, 1, 1, -1, -1, 1);
//	viewMatrix.ToIdentity();
//	UpdateShaderMatrices(quad->GetShader());
//	glDisable(GL_DEPTH_TEST);
//
//	glUniform2f(glGetUniformLocation(quad->GetShader()->GetProgram(), "pixelSize"), 1.0f / width, 1.0f / height);
//
//	for (int i = 0; i < POST_PASSES; ++i) {
//		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, bufferColourTex[1], 0);
//
//		glUniform1i(glGetUniformLocation(quad->GetShader()->GetProgram(), "isVertical"), 0);
//
//		quad->GetMesh()->SetTexture(bufferColourTex[1]);
//		quad->Draw(*this);
//	}
//
//	glBindFramebuffer(GL_FRAMEBUFFER, 0);
//	glUseProgram(0);
//	glEnable(GL_DEPTH_TEST);
//
//}
//
void Renderer::DrawScene() {
	
	BuildNodeLists(root);
	SortNodeLists();

	glBindFramebuffer(GL_FRAMEBUFFER, blur->GetFrameBuffer());
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glScissor(0, 0, width, height);//Sets the scissor region to the whole screen before the clear
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	if (usingScissor) {

		glScissor((float)width / 2.5f, (float)height / 2.5f,
			(float)width / 5.0f, (float)height / 5.0f);
	}


	DrawNodes();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	SwapBuffers();
	ClearNodeLists();

	
}

