#pragma once
#include "light.h"
#include "../object.h"

class SpotLight :public Light, public Object {
public:
	SpotLight();
	~SpotLight();

public:
	glm::vec3	mTargetDirection{ -1.0f };//探照灯的朝向
	float		mVisibleAngle{ 0.0f };    //可视角度的一半
};
