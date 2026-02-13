#pragma once
#include "light.h"

class DirectionalLight:public Light{
public:
	DirectionalLight();
	~DirectionalLight();
public:
	glm::vec3 mDirection{-1.0};//平行光只有方向没有位置
};
