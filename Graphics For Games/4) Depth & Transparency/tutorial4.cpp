#pragma comment(lib, "nclgl.lib")

#include "../../nclGL/window.h"
#include "../../nclgl/Renderer.h"

int main()	{
	Window w("Depth and Transparency!", 800 , 600, false);//This is all boring win32 window creation stuff!
	if(!w.HasInitialised()) {				//This shouldn't happen!
		return -1;
	}

	Renderer renderer(w);					//This handles all the boring OGL 3.2 stuff, and sets up our tutorial!
	if(!renderer.HasInitialised()) {		//This shouldn't happen!
		return -1;
	}

	Shader* triangleShader = new Shader(SHADERDIR"textureMatVert.vert", SHADERDIR"sceneFrag.frag");
	triangleShader->LinkProgram();

	Texture* brick = new Texture(TEXTUREDIR"brick.tga");
	Texture* glass = new Texture(TEXTUREDIR"stainedglass.tga");

	Mesh* squareMesh = Mesh::GenerateQuad();
	Mesh* triangleMesh = Mesh::GenerateTriangle();

	SceneNode* triangle = new SceneNode(triangleShader, triangleMesh);
	triangle->SetTexture(brick);

	SceneNode* square = new SceneNode(triangleShader, squareMesh);
	square->SetTransform(Matrix4::Translation(Vector3(1.0f, 0.0f, 2.0f)));
	square->SetTexture(glass);

	renderer.AttachSceneGraph(triangle);
	renderer.AttachSceneGraph(square);

	float zPos = -1.0f;

	while(w.UpdateWindow() && !Window::GetKeyboard()->KeyDown(KEYBOARD_ESCAPE)){				//And enter a while loop that renders the scene
		if(Window::GetKeyboard()->KeyTriggered(KEYBOARD_1)) {
			//renderer.ToggleObject();
		}
		if(Window::GetKeyboard()->KeyTriggered(KEYBOARD_2)) {
			renderer.ToggleDepth();
		}
		if(Window::GetKeyboard()->KeyTriggered(KEYBOARD_3)) {
			renderer.ToggleAlphaBlend();
		}
		if(Window::GetKeyboard()->KeyTriggered(KEYBOARD_4)) {
			renderer.ToggleBlendMode();
		}

		if(Window::GetKeyboard()->KeyDown(KEYBOARD_UP)) {
			zPos-= 0.1f;
		}
		if(Window::GetKeyboard()->KeyDown(KEYBOARD_DOWN)) {
			zPos+= 0.1f;
		}

		square->SetTransform(Matrix4::Translation(Vector3(0, 0, zPos)));

		renderer.UpdateScene(w.GetTimer()->GetTimedMS());
		renderer.RenderScene();
	}

	delete triangleShader;
	delete brick;
	delete glass;
	delete triangleMesh;
	delete squareMesh;

	return 0;
}