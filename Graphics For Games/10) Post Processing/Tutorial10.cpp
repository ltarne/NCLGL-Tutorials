#include "../../nclgl/window.h"
#include "../../nclgl/Renderer.h"
#include "../../nclgl/HeightMap.h"
#include "../../nclgl/OBJMesh.h"

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
	OBJMesh* cube = new OBJMesh(MESHDIR"cube.obj");

	SceneNode* node = new SceneNode(shader, heightMap, Vector4(1, 1, 1, 1));
	Texture* tex = new Texture(TEXTUREDIR "Barren Reds.JPG");
	tex->ToggleRepeating();
	node->SetTexture(tex);
	node->SetBoundingRadius(10000.0f);
	node->SetTransform(Matrix4::Translation(Vector3(-((257 * 16.0f) / 2), -300, -((257 * 16.0f) / 2))));

	Shader* processShader = new Shader(SHADERDIR"sceneVert.vert", SHADERDIR"sobel.frag");
	if (!processShader->LinkProgram()) {
		return -1;
	}

	Shader* sceneShader = new Shader(SHADERDIR"sceneVert.vert", SHADERDIR"sceneFrag.frag");

	if (!sceneShader->LinkProgram()) {
		return -1;
	}

	PostProcessingEffect::GenerateQuad();
	PostProcessingEffect* blur = new PostProcessingEffect(800, 600, sceneShader, processShader);

	renderer.AttachSceneGraph(node);
	renderer.AttachPostProcessingEffect(blur);

	while(w.UpdateWindow() && !Window::GetKeyboard()->KeyDown(KEYBOARD_ESCAPE)){
		renderer.UpdateScene(w.GetTimer()->GetTimedMS());
		renderer.RenderScene();
	}

	delete blur;
	delete heightMap;
	delete shader;
	delete tex;

	return 0;
}