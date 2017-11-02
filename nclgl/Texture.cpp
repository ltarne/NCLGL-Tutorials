#include "Texture.h"


Texture::Texture(string filePath) {
	texture = SOIL_load_OGL_texture(filePath.c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	repeating = false;
	filtering = false;
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
