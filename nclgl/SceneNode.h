#pragma once
#include "Mesh.h"
#include "Texture.h"

class SceneNode
{
public:
	SceneNode(Shader* shader, Mesh* mesh = nullptr, Vector4 colour = Vector4(1,1,1,1));
	~SceneNode();

	inline void			SetTransform(const Matrix4 &transform)	{ this->transform = transform; }
	inline const Matrix4&	GetTransform()					const	{ return transform; }
	inline Matrix4			GetWorldTransform()				const	{ return worldTransform; }

	inline void		SetScale(Matrix4 scale) { this->scale = scale; }
	inline void		SetScale(Vector3 scale) { this->scale = Matrix4::Scale(scale); }
	inline Matrix4	GetScale()			 const { return scale; }

	inline void		SetRotation(Matrix4 rotation) { this->rotation = rotation; }
	inline Matrix4	GetRotation()			 const { return rotation; }

	inline void		SetColour(Vector4 colour) { this->colour = colour; }
	inline Vector4	GetColour()			const { return colour; }

	inline void		SetMesh(Mesh* mesh) { this->mesh = mesh; }
	inline Mesh*	GetMesh()	const	{ return mesh; }

	inline void		SetShader(Shader* shader) { this->shader = shader; }
	inline Shader*	GetShader() const { return shader; }

	inline void		SetTexture(Texture* texture) {
		this->texture = texture; 
		mesh->SetTexture(*texture->GetTexture());
	}
	inline Texture*	GetTexture() { return texture; }

	inline void SetVisible(bool visible) { this->visible = visible; }
	inline bool GetVisible() const { return visible; }

	inline float GetBoundingRadius() const { return boundingRadius; }
	inline void SetBoundingRadius(float boundingRadius) { this->boundingRadius = boundingRadius; }

	inline float GetCameraDistance() const { return distanceFromCamera; }
	inline void SetCameraDistance(float distance) { this->distanceFromCamera = distance; }

	static bool CompareByCameraDistance(SceneNode* a, SceneNode* b) {
		return (a->distanceFromCamera < b->distanceFromCamera) ? true : false;
	}

	void LoadUniforms();

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

	Shader* shader;
	Mesh* mesh;
	Texture* texture;
	Vector4 colour;

	float distanceFromCamera;
	float boundingRadius;

	bool visible;
	
};

