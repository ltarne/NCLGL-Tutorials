#pragma comment(lib, "nclgl.lib")

#include "../../NCLGL/window.h"
#include "../../nclgl/Renderer.h"

int main() {
	Window w("Per Pixel Lighting!", 800,600,false);
	if(!w.HasInitialised()) {
		return -1;
	}
	
	Renderer renderer(w);//This handles all the boring OGL 3.2 initialisation stuff, and sets up our tutorial!
	if(!renderer.HasInitialised()) {
		return -1;
	}

	w.LockMouseToWindow(true);
	w.ShowOSPointer(false);

	Shader* shader = new Shader(SHADERDIR"lightVert.vert", SHADERDIR"lightFrag.frag");
	shader->LinkProgram();

	HeightMap* heightMap = new HeightMap(TEXTUREDIR "terrain.raw");

	SceneNode* node = new SceneNode(shader, heightMap, Vector4(1, 1, 1, 1));
	Texture* tex = new Texture(TEXTUREDIR "Barren Reds.JPG");
	
	tex->ToggleRepeating();

	Texture* bumpTex = new Texture(TEXTUREDIR"Barren RedsDOT3.JPG");
	bumpTex->ToggleRepeating();

	node->SetTexture(tex);
	node->SetBumpTexture(bumpTex);

	node->SetBoundingRadius(10000.0f);
	node->SetTransform(Matrix4::Translation(Vector3(-((257 * 16.0f) / 2), -300, -((257 * 16.0f) / 2))));
	

	renderer.AttachSceneGraph(node);

	while(w.UpdateWindow() && !Window::GetKeyboard()->KeyDown(KEYBOARD_ESCAPE)){
		renderer.UpdateScene(w.GetTimer()->GetTimedMS());
		renderer.RenderScene();
	}

	delete heightMap;
	delete shader;
	delete tex;
	delete bumpTex;

	return 0;
}