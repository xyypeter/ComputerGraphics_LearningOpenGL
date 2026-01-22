#pragma once

#include "camera.h"

class PerspectiveCamera :public Camera {
public:
	PerspectiveCamera(float fovy, float aspect, float near, float far);
	~PerspectiveCamera();
	glm::mat4 getProjectionMatrix()override;
	void scale(float deltaScale)override;
private:
	float mFovy = 0.0f;   //视张角
	float mAspect = 0.0f; //横纵坐标比
	float mNear = 0.0f;   //近平面距离相机的距离
	float mFar = 0.0f;    //远平面距离相机的距离
};


