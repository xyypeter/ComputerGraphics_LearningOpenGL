#pragma once
#include "../core.h"

//使用C++的枚举类型
enum class MaterialType {
	PhongMaterial
};

class Material {
public:
	Material();
	~Material();

public:
	MaterialType mType;
};