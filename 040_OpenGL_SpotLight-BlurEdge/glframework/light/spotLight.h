#pragma once
#include "light.h"
#include "../object.h"

class SpotLight :public Light, public Object {
public:
	SpotLight();
	~SpotLight();

public:
	glm::vec3	mTargetDirection{ -1.0f };//探照灯的朝向
	//float		mVisibleAngle{ 0.0f };    //可视角度的一半
	float		mInnerAngle{ 0.0f };      //内层角度(最小可视范围角度)
	float		mOuterAngle{ 0.0f };      //外层角度(最大可是范围角度)
};
