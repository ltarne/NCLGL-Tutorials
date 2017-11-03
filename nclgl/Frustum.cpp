#include "Frustum.h"



Frustum::Frustum() {
}


Frustum::~Frustum() {
}

void Frustum::FromMatrix(const Matrix4 & mvp) {
	Vector3 xAxis = Vector3(mvp.values[0], mvp.values[4], mvp.values[8]);
	Vector3 yAxis = Vector3(mvp.values[1], mvp.values[5], mvp.values[9]);
	Vector3 zAxis = Vector3(mvp.values[2], mvp.values[6], mvp.values[10]);
	Vector3 wAxis = Vector3(mvp.values[3], mvp.values[7], mvp.values[11]);

	//Right
	planes[0] = Plane(wAxis - xAxis, mvp.values[15] - mvp.values[12], true);

	//Left
	planes[1] = Plane(wAxis - xAxis, mvp.values[15] + mvp.values[12], true);

	//Bottom
	planes[2] = Plane(wAxis - xAxis, mvp.values[15] + mvp.values[13], true);

	//Top
	planes[3] = Plane(wAxis - xAxis, mvp.values[15] - mvp.values[13], true);

	//Far
	planes[4] = Plane(wAxis - xAxis, mvp.values[15] - mvp.values[14], true);

	//Near
	planes[5] = Plane(wAxis - xAxis, mvp.values[15] + mvp.values[14], true);
}

bool Frustum::InsideFrustrum(SceneNode & node) {
	for (int p = 0; p < 6; ++p) {
		if (!planes[p].SphereInPlane(node.GetWorldTransform().GetPositionVector(), node.GetBoundingRadius())) {
			return false;
		}
	}
	return true;
}
