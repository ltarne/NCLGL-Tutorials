#include "StencilNode.h"




StencilNode::StencilNode(Shader * shader, Mesh * mesh, Vector4 colour) 
	:SceneNode(shader, mesh, colour) {

}

StencilNode::~StencilNode()
{
}

void StencilNode::ActivateSpecialSettings() {
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	glStencilFunc(GL_ALWAYS, 2, ~0);
	glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);
}

void StencilNode::DeactivateSpecialSettings() {
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glStencilFunc(GL_EQUAL, 2, ~0);
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
}
