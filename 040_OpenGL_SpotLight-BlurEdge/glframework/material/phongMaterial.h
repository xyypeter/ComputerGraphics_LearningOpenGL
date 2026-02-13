#pragma once
#include "material.h"
#include "../texture.h"

class PhongMaterial :public Material {
public:
	PhongMaterial();
	~PhongMaterial();

public:
	//表示漫反射diffuse贴图,主要功能​​是提供物体表面的基础颜色(漫反射颜色),​​在光照计算中​​决定了物体表面对直接光照的反射特性
	//在基础的光照模型中,环境光分量经常直接使用漫反射颜色
	Texture*	mDiffuse{ nullptr };

	//高光蒙版的Texture对象
	Texture*    mSpecularMask{ nullptr };

	//高光反射,控制物体表面高光区域的大小和集中程度(就是之前的次方数)
	//低Shininess值（如5-20）：产生​​大而柔和​​的高光区域
	//高Shininess值（如50-128）：产生​​小而锐利​​的高光区域
	//高光强度 = (反射向量 · 视线向量)^Shininess
	//Shininess作为指数，控制高光的衰减速度
	float		mShiness{ 1.0f };
};
