#include "Texture.h"

const GLenum Texture::textureUnits[] = { GL_TEXTURE0, GL_TEXTURE1, GL_TEXTURE2, GL_TEXTURE3, GL_TEXTURE4, GL_TEXTURE5, GL_TEXTURE6, GL_TEXTURE7 };

Texture::Texture(string filePath, int num, string name) {
	texture = SOIL_load_OGL_texture(filePath.c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	repeating = false;
	filtering = false;
	this->name = name;
	textureMatrix = Matrix4();
	this->num = num;
}

Texture::Texture(string* filePaths, int num, string name) {
	texture = SOIL_load_OGL_cubemap(filePaths[0].c_str(), filePaths[1].c_str(), filePaths[2].c_str(), filePaths[3].c_str(), filePaths[4].c_str(), filePaths[5].c_str(),
		SOIL_LOAD_RGB, SOIL_CREATE_NEW_ID, 0);

	this->name = name;
	this->num = num;
}

Texture::~Texture() {

}

void Texture::ToggleRepeating() {
	repeating = !repeating;
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, repeating ? GL_REPEAT : GL_CLAMP); //x axis
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, repeating ? GL_REPEAT : GL_CLAMP); //y axis

	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::ToggleFiltering() {
	filtering = !filtering;
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filtering ? GL_LINEAR : GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filtering ? GL_LINEAR : GL_NEAREST);

	glBindTexture(GL_TEXTURE_2D, 0);

}

void Texture::RotateMatrix(float angle) {
	Matrix4 pushPos = Matrix4::Translation(Vector3(0.5f, 0.5f, 0));
	Matrix4 popPos = Matrix4::Translation(Vector3(-0.5f, -0.5f, 0));
	Matrix4 rotation = Matrix4::Rotation(angle, Vector3(0, 0, 1));
	textureMatrix = pushPos * rotation * popPos;
}
