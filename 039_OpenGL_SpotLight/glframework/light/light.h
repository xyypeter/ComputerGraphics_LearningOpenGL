#pragma once
#include "../core.h"

class Light {
public:
	Light();
	~Light();
public:
	glm::vec3	mColor{ 1.0f };            //光的颜色,白光
	float		mSpecularIntensity{ 1.0f };//镜面反射光的强度大小
};