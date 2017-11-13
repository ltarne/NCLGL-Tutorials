#pragma comment(lib, "nclgl.lib")

#include "../NCLGL/window.h"
#include "../../nclgl/Renderer.h"

int main() {
	Window w("Shadow Mapping! MD5 mesh courtesy of http://www.katsbits.com/", 800,600,false); //This is all boring win32 window creation stuff!
	if(!w.HasInitialised()) {
		return -1;
	}
	
	Renderer renderer(w); //This handles all the boring OGL 3.2 initialisation stuff, and sets up our tutorial!
	if(!renderer.HasInitialised()) {
		return -1;
	}

	w.LockMouseToWindow(true);
	w.ShowOSPointer(false);


	Shader* shader = new Shader(SHADERDIR"shadowSkeletonVertex.glsl", SHADERDIR"shadowSkeletonFrag.frag");
	shader->LinkProgram();

	Shader* sceneShader = new Shader(SHADERDIR"shadowSceneVertex.glsl", SHADERDIR"shadowSceneFrag.frag");
	sceneShader->LinkProgram();

	Shader* processShader = new Shader(SHADERDIR"shadowVertex.glsl", SHADERDIR"shadowFrag.frag");
	processShader->LinkProgram();

	Texture* bricks = new Texture(TEXTUREDIR"bricks.tga");

	MD5FileData*	hellData = new MD5FileData(MESHDIR"hellknight.md5mesh");
	MD5Node*		hellNode = new MD5Node(*hellData);
	hellNode->SetShader(shader);
	hellNode->SetBoundingRadius(1000.0f);

	hellData->AddAnim(MESHDIR"idle2.md5anim");
	hellNode->PlayAnim(MESHDIR"idle2.md5anim");


	SceneNode* quad = new SceneNode(sceneShader, Mesh::GenerateQuad());
	quad->AddTexture(bricks);

	PostProcessingEffect::GenerateQuad();
	ShadowEffect* shadow = new ShadowEffect(800, 600, sceneShader, processShader);

	renderer.AttachSceneGraph(hellNode);
	renderer.AttachSceneGraph(quad);
	renderer.AttachPostProcessingEffect(shadow);

	while(w.UpdateWindow() && !Window::GetKeyboard()->KeyDown(KEYBOARD_ESCAPE)){
		renderer.UpdateScene(w.GetTimer()->GetTimedMS());
		renderer.RenderScene();
	}

	return 0;
}