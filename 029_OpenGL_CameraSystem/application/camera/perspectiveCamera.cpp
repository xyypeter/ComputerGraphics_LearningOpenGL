#include "perspectiveCamera.h"

PerspectiveCamera::PerspectiveCamera(float fovy, float aspect, float near, float far) {
	mFovy = fovy;
	mAspect = aspect;
	mNear = near;
	mFar = far;
}
PerspectiveCamera::~PerspectiveCamera() {}

glm::mat4 PerspectiveCamera::getProjectionMatrix() {
	//注意：传入的是fovy的角度，需要转化为弧度
	return glm::perspective(glm::radians(mFovy), mAspect, mNear, mFar);
}

void PerspectiveCamera::scale(float deltaScale) {
	auto front = glm::cross(mUp, mRight);//透视投影相机的缩放，本质上是沿着front向量移动的运动相机，使用mUp向量叉乘mRight向量，得到front向量
	mPosition += (front * deltaScale);   //使用累加的效果
}



