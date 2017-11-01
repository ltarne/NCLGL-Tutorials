#include "Mesh.h"



Mesh::Mesh() {
	for (int i = 0; i < MAX_BUFFER; ++i) {
		bufferObject[i] = 0;
	}

	glGenVertexArrays(1, &arrayObject);

	numVertices		= 0;
	vertices		= NULL;
	colours			= NULL;
	texture			= 0;
	textureCoords	= NULL;
	type			= GL_TRIANGLES;
	
}


Mesh::~Mesh() {
	glDeleteVertexArrays(1, &arrayObject);
	glDeleteBuffers(MAX_BUFFER, bufferObject);

	glDeleteTextures(1, &texture);
	delete[] textureCoords;

	delete[] vertices;
	delete[] colours;
}

void Mesh::Draw() {
	//Work on this array object
	glBindTexture(GL_TEXTURE_2D, texture);
	glBindVertexArray(arrayObject);
	//Draw
	glDrawArrays(type, 0, numVertices);
	//Stop working on this array object
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

Mesh* Mesh::GenerateTriangle() {
	Mesh* m = new Mesh();
	m->numVertices = 3;

	m->vertices = new Vector3[m->numVertices];
	m->vertices[0] = Vector3(0.0f, 0.5f, 0.0f); //Top Middle
	m->vertices[1] = Vector3(0.5f, -0.5f, 0.0f); //Bottom Right
	m->vertices[2] = Vector3(-0.5f, -0.5f, 0.0f); //Bottom Left

	m->textureCoords = new Vector2[m->numVertices];
	m->textureCoords[0] = Vector2(0.5f, 0.0f);
	m->textureCoords[1] = Vector2(1.0f, 1.0f);
	m->textureCoords[2] = Vector2(0.0f, 1.0f);

	m->colours = new Vector4[m->numVertices];
	m->colours[0] = Vector4(1.0f, 0.0f, 0.0f, 1.0f); //Red
	m->colours[1] = Vector4(0.0f, 1.0f, 0.0f, 1.0f); //Green
	m->colours[2] = Vector4(0.0f, 0.0f, 1.0f, 1.0f); //Blue

	m->BufferData();

	return m;
}

void Mesh::BufferData() {
	//Bind the vertex array object to ARRAY_BUFFER
	glBindVertexArray(arrayObject);

	//Setup vertex position attributes
	glGenBuffers(1, &bufferObject[VERTEX_BUFFER]);
	glBindBuffer(GL_ARRAY_BUFFER, bufferObject[VERTEX_BUFFER]);
	glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(Vector3), vertices, GL_STATIC_DRAW);
	
	glVertexAttribPointer(VERTEX_BUFFER, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(VERTEX_BUFFER);

	if (textureCoords) {
		glGenBuffers(1, &bufferObject[TEXTURE_BUFFER]);
		glBindBuffer(GL_ARRAY_BUFFER, bufferObject[TEXTURE_BUFFER]);
		glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(Vector2), textureCoords, GL_STATIC_DRAW);

		glVertexAttribPointer(TEXTURE_BUFFER, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(TEXTURE_BUFFER);
	}

	//Setup colour attribute
	if (colours) {
		glGenBuffers(1, &bufferObject[COLOUR_BUFFER]);
		glBindBuffer(GL_ARRAY_BUFFER, bufferObject[COLOUR_BUFFER]);
		glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(Vector4), colours, GL_STATIC_DRAW);

		glVertexAttribPointer(COLOUR_BUFFER, 4, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(COLOUR_BUFFER);
	}
	//Stop working on this vertex array
	glBindVertexArray(0);


}
