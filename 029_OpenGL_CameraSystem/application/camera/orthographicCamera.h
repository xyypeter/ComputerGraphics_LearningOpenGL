#pragma once
#include "camera.h"

class OrthographicCamera :public Camera {
public:
	OrthographicCamera(float l, float r, float t, float b, float n, float f);
	~OrthographicCamera();
	glm::mat4 getProjectionMatrix()override;
	void scale(float deltaScale)override;
private:
	float mLeft = 0.0f;  //左
	float mRight = 0.0f; //右
	float mTop = 0.0f;   //顶部
	float mBottom = 0.0f;//底部
	float mNear = 0.0f;  //近
	float mFar = 0.0f;   //远
	float mScale{ 0.0f };//缩放因子(描述变大变小的程度)
};



