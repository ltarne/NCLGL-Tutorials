#include "../../nclgl/window.h"
#include "../../nclgl/Renderer.h"
#include "CubeRobot.h"

#pragma comment(lib, "nclgl.lib")

int main() {	
	Window w("Scene Graphs!", 800,600,false);
	if(!w.HasInitialised()) {
		return -1;
	}

	Renderer renderer(w);
	if(!renderer.HasInitialised()) {
		return -1;
	}

	w.LockMouseToWindow(true);
	w.ShowOSPointer(false);

	Shader* shader = new Shader(SHADERDIR"sceneVert.vert", SHADERDIR"sceneFrag.frag");
	shader->LinkProgram();

	CubeRobot::CreateCube();
	CubeRobot* cubeRobot = new CubeRobot(shader);
	cubeRobot->SetTransform(Matrix4::Translation(Vector3(0, 0, -10)));
	//cubeRobot->SetScale(Vector3(0.01, 0.01, 0.01));

	renderer.AttachSceneGraph(cubeRobot);

	while(w.UpdateWindow() && !Window::GetKeyboard()->KeyDown(KEYBOARD_ESCAPE)){
		renderer.UpdateScene(w.GetTimer()->GetTimedMS());
		renderer.RenderScene();
	}

	delete shader;
	CubeRobot::DeleteCube();

	return 0;
}