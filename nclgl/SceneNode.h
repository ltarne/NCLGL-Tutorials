#pragma once
#include "Matrix4.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Mesh.h"

class SceneNode
{
public:
	SceneNode(Mesh* m = nullptr, Vector4 colour = Vector4(1,1,1,1));
	~SceneNode();

	inline void			SetTransform(const Matrix4 &transform)	{ this->transform = transform; }
	inline const Matrix4&	GetTransform()					const	{ return transform; }
	inline Matrix4			GetWorldTransform()				const	{ return worldTransform; }

	inline void SetScale(Matrix4 scale) { this->scale = scale; }
	inline Matrix4 GetScale()			 const { return scale; }

	inline void SetRotation(Matrix4 rotation) { this->rotation = rotation; }
	inline Matrix4 GetRotation()			 const { return rotation; }

	inline void	SetColour(Vector4 colour) { this->colour = colour; }
	inline Vector4 GetColour()			const { return colour; }

	inline void	SetMesh(Mesh* mesh) { this->mesh = mesh; }
	inline Mesh*	GetMesh()	const	{ return mesh; }

	void AddChild(SceneNode* child);

	virtual void Update(float msec);
	virtual void Draw(const OGLRenderer &renderer);

	inline std::vector<SceneNode*>::const_iterator GetChildIteratorStart()	{ return children.begin(); }
	inline std::vector<SceneNode*>::const_iterator GetChildIteratorEnd()	{ return children.end(); }


protected:
	SceneNode* parent;
	std::vector<SceneNode*> children;

	Matrix4 worldTransform;
	Matrix4 transform;
	Matrix4 rotation;
	Matrix4 scale;
	//Vector3 modelScale;

	Mesh* mesh;
	Vector4 colour;
	
};

