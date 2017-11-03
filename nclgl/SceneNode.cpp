#include "SceneNode.h"


SceneNode::SceneNode(Shader* shader, Mesh * mesh, Vector4 colour) {
	this->shader	= shader;
	this->mesh		= mesh;
	this->colour	= colour;

	parent			= NULL;
	transform	= Matrix4();
	rotation	= Matrix4();
	scale		= Matrix4::Scale(Vector3(1, 1, 1));

	visible = true;

	boundingRadius = 1.0f;
	distanceFromCamera = 0.0f;

}

SceneNode::~SceneNode() {
	for (unsigned int i = 0; i < children.size(); ++i) {
		delete children[i];
	}
}

void SceneNode::LoadUniforms() {
	//Texture Uniforms
	/*glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->GetTexture());*/
	glUniform1i(glGetUniformLocation(shader->GetProgram(), "tex"), 0);
	

	//Transform
	glUniformMatrix4fv(glGetUniformLocation(shader->GetProgram(), "modelMatrix"), 1, false, (float*)&worldTransform);

	//Texture Matrix
	glUniformMatrix4fv(glGetUniformLocation(shader->GetProgram(), "textureMatrix"), 1, false, (float*)&texture->GetTextureMatrix());

	//Colour
	glUniform4fv(glGetUniformLocation(shader->GetProgram(), "nodeColour"), 1, (float*)&colour);
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
	LoadUniforms();
	
	if (mesh != nullptr) {
		mesh->Draw();
	}
}
