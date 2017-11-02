#pragma comment(lib, "nclgl.lib")

#include "../../nclGL/window.h"
#include "../../nclgl/Renderer.h"

int main() {
	Window w("Texturing!", 800,600,false);	 //This is all boring win32 window creation stuff!
	if(!w.HasInitialised()) {
		return -1;
	}
	
	Renderer renderer(w);	//This handles all the boring OGL 3.2 initialisation stuff, and sets up our tutorial!
	if(!renderer.HasInitialised()) {
		return -1;
	}

	Shader* triangleShader = new Shader(SHADERDIR"textureMatVert.vert", SHADERDIR"sceneFrag.frag");
	triangleShader->LinkProgram();

	Texture* brick = new Texture(TEXTUREDIR"brick.tga");

	SceneNode* triangle = new SceneNode(triangleShader, Mesh::GenerateTriangle());
	
	triangle->SetTexture(brick);

	renderer.AttachSceneGraph(triangle);

	float rotate = 0.0f;
	while(w.UpdateWindow() && !Window::GetKeyboard()->KeyDown(KEYBOARD_ESCAPE)){
		if(Window::GetKeyboard()->KeyDown(KEYBOARD_LEFT) ) {
			--rotate;
			triangle->GetTexture()->RotateMatrix(rotate);
		}

		if(Window::GetKeyboard()->KeyDown(KEYBOARD_RIGHT) ) {
			++rotate;
			triangle->GetTexture()->RotateMatrix(rotate);
		}

		if(Window::GetKeyboard()->KeyTriggered(KEYBOARD_1) ) {
			triangle->GetTexture()->ToggleFiltering();
		}

		if(Window::GetKeyboard()->KeyTriggered(KEYBOARD_2) ) {
			triangle->GetTexture()->ToggleRepeating();
		}

		renderer.UpdateScene(w.GetTimer()->GetTimedMS());
		renderer.RenderScene();
	}

	delete triangleShader;
	delete brick;

	return 0;
}