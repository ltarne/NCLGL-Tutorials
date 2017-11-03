#include "../../nclGL/window.h"
#include "../../nclgl/Renderer.h"


#pragma comment(lib, "nclgl.lib")

int main() {	
	Window w("Scissors and Stencils!", 800,600,false);	//This is all boring win32 window creation stuff!
	if(!w.HasInitialised()) {
		return -1;
	}

	w.LockMouseToWindow(true);
	w.ShowOSPointer(false);
	
	Renderer renderer(w);	//This handles all the boring OGL 3.2 initialisation stuff, and sets up our tutorial!
	if(!renderer.HasInitialised()) {
		return -1;
	}

	Shader* triangleShader = new Shader(SHADERDIR"textureMatVert.vert", SHADERDIR"stencilFrag.frag");
	triangleShader->LinkProgram();

	Texture* brick = new Texture(TEXTUREDIR"brick.tga");
	Texture* chessboard = new Texture(TEXTUREDIR"chessboard.tga");

	Mesh* squareMesh = Mesh::GenerateQuad();
	Mesh* triangleMesh = Mesh::GenerateTriangle();

	SceneNode* triangle = new SceneNode(triangleShader, triangleMesh);
	triangle->SetTexture(brick);

	StencilNode* square = new StencilNode(triangleShader, squareMesh);
	square->SetTransform(Matrix4::Translation(Vector3(0.0f, 0.0f, 0.0f)));
	square->SetTexture(chessboard);
	square->SetVisible(false);

	renderer.AttachSceneGraph(square);
	renderer.AttachSceneGraph(triangle);
	

	renderer.ToggleDepth();

	while(w.UpdateWindow() && !Window::GetKeyboard()->KeyDown(KEYBOARD_ESCAPE)){
		if(Window::GetKeyboard()->KeyTriggered(KEYBOARD_1)) {
			renderer.ToggleScissor();
		}
		if(Window::GetKeyboard()->KeyTriggered(KEYBOARD_2)) {
			renderer.ToggleStencil();
			square->SetVisible(!square->GetVisible());
		}

		renderer.UpdateScene(w.GetTimer()->GetTimedMS());
		renderer.RenderScene();
	}

	delete triangleShader;
	delete brick;
	delete chessboard;
	delete triangleMesh;
	delete squareMesh;

	return 0;
}