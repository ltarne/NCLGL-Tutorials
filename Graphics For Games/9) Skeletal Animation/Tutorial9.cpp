#pragma comment(lib, "nclgl.lib")

#include "../../nclgl/window.h"
#include "../../nclgl/Renderer.h"

int main() {
	Window w("Skeletal Animation!", 800,600,false);
	if(!w.HasInitialised()) {
		return -1;
	}
	
	Renderer renderer(w);//This handles all the boring OGL 3.2 initialisation stuff, and sets up our tutorial!
	if(!renderer.HasInitialised()) {
		return -1;
	}

	w.LockMouseToWindow(true);
	w.ShowOSPointer(false);

	Shader* shader = new Shader(SHADERDIR"skeletonVertex.glsl", SHADERDIR"skeletonFrag.frag");
	Shader* triangleShader = new Shader(SHADERDIR"sceneVert.vert", SHADERDIR"sceneFrag.frag");
	triangleShader->LinkProgram();
	//Shader* shader = new Shader(SHADERDIR"TexturedVertex.glsl", SHADERDIR"TexturedFragment.glsl");
	if (!shader->LinkProgram()) {
		return-1;
	}

	MD5FileData*	hellData = new MD5FileData(MESHDIR"hellknight.md5mesh");
	SceneNode* parentNode = new SceneNode(triangleShader, Mesh::GenerateTriangle());


	MD5Node*		hellNode = new MD5Node(*hellData);
	parentNode->AddChild(hellNode);
	hellNode->SetShader(shader);
	hellNode->SetBoundingRadius(1000.0f);
	/*hellData->AddAnim(MESHDIR"idle2.md5anim");
	hellNode->PlayAnim(MESHDIR"idle2.md5anim");*/

	hellData->AddAnim(MESHDIR"walk7.md5anim");
	hellNode->PlayAnim(MESHDIR"walk7.md5anim");
	parentNode->SetTransform(Matrix4::Translation(Vector3(0.0f, -10.0f, -10.0f)));

	renderer.AttachSceneGraph(parentNode);
	Vector3 position = Vector3(0.0f, 0.0f, -10.0f);

	while(w.UpdateWindow() && !Window::GetKeyboard()->KeyDown(KEYBOARD_ESCAPE)){
		if (Window::GetKeyboard()->KeyDown(KEYBOARD_UP)) {
			position.z += 0.5f;
		}
		else if (Window::GetKeyboard()->KeyDown(KEYBOARD_UP)) {
			position.z -= 0.5f;
		}
		parentNode->SetTransform(Matrix4::Translation(position));
		renderer.UpdateScene(w.GetTimer()->GetTimedMS());
		renderer.RenderScene();
	}
	
	delete shader;

	return 0;
}