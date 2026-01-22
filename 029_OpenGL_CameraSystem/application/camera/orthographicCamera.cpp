#include "orthographicCamera.h"

OrthographicCamera::OrthographicCamera(float l, float r, float t, float b, float n, float f) {
	mLeft = l;
	mRight = r;
	mTop = t;
	mBottom = b;
	mNear = n;
	mFar = f;
}
OrthographicCamera::~OrthographicCamera() {}

glm::mat4 OrthographicCamera::getProjectionMatrix(){
	float scale = std::pow(2.0f, mScale);
	//指数缩放通常用于正交相机的视野大小调整，因为它更符合人眼对物体尺寸变化的感知。
	//人眼对于物体的尺寸变化是非线性的，指数缩放能更平滑地适应不同的视距需求
	return glm::ortho(mLeft * scale, mRight * scale, mBottom * scale, mTop * scale, mNear, mFar);
	//mLeft, mRight, mBottom, mTop 是定义了相机视野边界的变量，分别表示视图的左、右、下、上边界
	//无需修改远近平面，因为调整远近平面可能引起对原有内容的剪裁发生 
	//将这些边界与 scale 相乘是为了根据 mScale 值调整视野的大小
	//这意味着当 mScale 增大时，相机的视野范围也会增大，反之则减小
}

void OrthographicCamera::scale(float deltaScale) {
	mScale += deltaScale;//正交投影相机缩放的本质是，变化了投影盒体的缩放大小
}


