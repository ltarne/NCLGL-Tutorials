#include "../../nclgl/window.h"
#include "../../nclgl/Renderer.h"
#include "../../nclgl/HeightMap.h"

#pragma comment(lib, "nclgl.lib")

int main() {	
	Window w("Post Processing!", 800,600,false);
	if(!w.HasInitialised()) {
		return -1;
	}
	
	Renderer renderer(w);
	if(!renderer.HasInitialised()) {
		return -1;
	}

	w.LockMouseToWindow(true);
	w.ShowOSPointer(false);

	Shader* shader = new Shader(SHADERDIR"sceneVert.vert", SHADERDIR"heightMapFrag.frag");
	shader->LinkProgram();

	HeightMap* heightMap = new HeightMap(TEXTUREDIR "terrain.raw");

	SceneNode* node = new SceneNode(shader, heightMap, Vector4(0, 1, 0, 1));
	Texture* tex = new Texture(TEXTUREDIR "Barren Reds.JPG");
	tex->ToggleRepeating();
	node->SetTexture(tex);
	node->SetBoundingRadius(10000.0f);
	node->SetTransform(Matrix4::Translation(Vector3(-((257 * 16.0f) / 2), -300, -((257 * 16.0f) / 2))));

	renderer.AttachSceneGraph(node);

	while(w.UpdateWindow() && !Window::GetKeyboard()->KeyDown(KEYBOARD_ESCAPE)){
		renderer.UpdateScene(w.GetTimer()->GetTimedMS());
		renderer.RenderPPScene();
	}

	delete heightMap;
	delete shader;
	delete tex;

	return 0;
}