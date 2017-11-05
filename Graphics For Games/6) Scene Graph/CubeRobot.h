#pragma once
#include "..\..\nclgl\SceneNode.h"
#include "..\..\nclgl\OBJMesh.h"
class CubeRobot :
	public SceneNode
{
public:
	CubeRobot(Shader* shader);
	~CubeRobot();

	virtual void Update(float msec);

	inline static void CreateCube() {
		OBJMesh* m = new OBJMesh();
		m->LoadOBJMesh(MESHDIR "cube.obj");
		cube = m;
	}

	inline static void DeleteCube() { delete cube; }

protected:
	static Mesh* cube;
	SceneNode* body;
	SceneNode* head;
	SceneNode* leftArm;
	SceneNode* rightArm;
	SceneNode* leftLeg;
	SceneNode* rightLeg;
};

