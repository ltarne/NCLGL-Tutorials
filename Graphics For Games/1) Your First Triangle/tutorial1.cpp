#pragma comment(lib, "nclgl.lib")

#include "../../NCLGL/window.h"
#include "Renderer.h"

int main()	{
	Window w("My First OpenGL 3 Triangle!", 800 , 600, false);

	if(!w.HasInitialised()) {
		return -1;
	}
	
	Renderer renderer(w);
	if(!renderer.HasInitialised()) {
		return -1;
	}

	while(w.UpdateWindow()  && !Window::GetKeyboard()->KeyDown(KeyboardKeys::KEYBOARD_ESCAPE)){
		renderer.RenderScene();
	}

	return 0;
}