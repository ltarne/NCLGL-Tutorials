#include "CubeRobot.h"

Mesh* CubeRobot::cube = NULL;

CubeRobot::CubeRobot(Shader* shader)
	: SceneNode(shader) {
	body = new SceneNode(shader, cube, Vector4(1, 0, 0, 1));
	body->SetScale(Vector3(10, 15, 5));
	body->SetTransform(Matrix4::Translation(Vector3(0, 35, 0)));
	body->SetBoundingRadius(15.0f);
	AddChild(body);

	head = new SceneNode(shader, cube, Vector4(0, 1, 0, 1));
	head->SetScale(Vector3(5, 5, 5));
	head->SetTransform(Matrix4::Translation(Vector3(0, 30, 0)));
	head->SetBoundingRadius(5.0f);
	body->AddChild(head);

	leftArm = new SceneNode(shader, cube, Vector4(0, 0, 1, 1));
	leftArm->SetScale(Vector3(3, -18, 3));
	leftArm->SetTransform(Matrix4::Translation(Vector3(-12, 30, -1)));
	leftArm->SetBoundingRadius(18.0f);
	body->AddChild(leftArm);

	rightArm = new SceneNode(shader, cube, Vector4(0, 0, 1, 1));
	rightArm->SetScale(Vector3(3, -18, 3));
	rightArm->SetTransform(Matrix4::Translation(Vector3(12, 30, -1)));
	rightArm->SetBoundingRadius(18.0f);
	body->AddChild(rightArm);

	leftLeg = new SceneNode(shader, cube, Vector4(0, 0, 1, 1));
	leftLeg->SetScale(Vector3(3, -17.5, 3));
	leftLeg->SetTransform(Matrix4::Translation(Vector3(-8, 0, 0)));
	leftLeg->SetBoundingRadius(18.0f);
	body->AddChild(leftLeg);

	rightLeg = new SceneNode(shader, cube, Vector4(0, 0, 1, 1));
	rightLeg->SetScale(Vector3(3, -17.5, 3));
	rightLeg->SetTransform(Matrix4::Translation(Vector3(8, 0, 0)));
	rightLeg->SetBoundingRadius(18.0f);
	body->AddChild(rightLeg);



}

CubeRobot::~CubeRobot() {

}

void CubeRobot::Update(float msec) {
	//transform = transform * Matrix4::Rotation(msec / 10.0f, Vector3(0, 1, 0));
	rotation = rotation * Matrix4::Rotation(msec / 10.0f, Vector3(0, 1, 0));
	//head->SetTransform(head->GetTransform() * Matrix4::Rotation(-msec / 10.0f, Vector3(0, 1, 0)));
	head->SetRotation(head->GetRotation() * Matrix4::Rotation(-msec / 10.0f, Vector3(0, 1, 0)));

	//leftArm->SetTransform(leftArm->GetTransform() * Matrix4::Rotation(-msec / 10.0f, Vector3(1, 0, 0)));
	leftArm->SetRotation(leftArm->GetRotation() * Matrix4::Rotation(-msec / 10.0f, Vector3(1, 0, 0)));
	//rightArm->SetTransform(rightArm->GetTransform() * Matrix4::Rotation(msec / 10.0f, Vector3(1, 0, 0)));
	rightArm->SetRotation(rightArm->GetRotation() * Matrix4::Rotation(msec / 10.0f, Vector3(1, 0, 0)));
	SceneNode::Update(msec);
}

