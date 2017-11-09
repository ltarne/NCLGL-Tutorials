#pragma comment(lib, "framework.lib")

#include "../../nclgl/window.h"
#include "../../nclgl/Renderer.h"

int main() {
	Window w("Cube Mapping! sky textures courtesy of http://www.hazelwhorley.com", 800,600,false);
	if(!w.HasInitialised()) {
		return -1;
	}
	
	Renderer renderer(w);
	if(!renderer.HasInitialised()) {
		return -1;
	}

	w.LockMouseToWindow(true);
	w.ShowOSPointer(false);

	Shader* shader = new Shader(SHADERDIR"lightVert.vert", SHADERDIR"lightFrag.frag");
	shader->LinkProgram();

	HeightMap* heightMap = new HeightMap(TEXTUREDIR "terrain.raw");
	Mesh* quad = Mesh::GenerateQuad();

	/*SceneNode* skybox = new SceneNode()*/

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