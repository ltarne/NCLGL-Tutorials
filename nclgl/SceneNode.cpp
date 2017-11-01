#include "SceneNode.h"


SceneNode::SceneNode(Shader* shader, Mesh * mesh, Vector4 colour) {
	this->shader	= shader;
	this->mesh		= mesh;
	this->colour	= colour;
	parent			= NULL;
	transform	= Matrix4::Translation(Vector3(0,0,0));
	rotation	= Matrix4::Rotation(180.0f, Vector3(0, 1, 0));
	scale		= Matrix4::Scale(Vector3(1, 1, 1));

}

SceneNode::~SceneNode() {
	for (unsigned int i = 0; i < children.size(); ++i) {
		delete children[i];
	}
}

void SceneNode::AddChild(SceneNode* child) {
	children.push_back(child);
	child->parent = this;
}

void SceneNode::Update(float msec) {
	transform = transform * rotation * scale;
	if (parent) {
		worldTransform = parent->worldTransform * transform;
	}
	else {
		worldTransform = transform;
	}
	for (vector<SceneNode*>::iterator i = children.begin(); i != children.end(); ++i) {
		(*i)->Update(msec);
	}
}

void SceneNode::Draw(const OGLRenderer &renderer) {

	glUniform1i(glGetUniformLocation(shader->GetProgram(), "tex"), 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->GetTexture());
	if (mesh != nullptr) {
		mesh->Draw();
	}
}
