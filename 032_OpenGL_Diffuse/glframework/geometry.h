#pragma once

#include "core.h"

class Geometry {
public:
	Geometry();
	~Geometry();

	static Geometry* createBox(float size);                 //创建立方体几何对象
	static Geometry* createSphere(float radius);            //创建球体几何对象
	static Geometry* createPlane(float width, float height);//创建平面几何对象

	GLuint getVao()const { return mVao; }
	uint32_t getIndicesCount()const { return mIndicesCount; }

private:
	GLuint mVao{ 0 };
	GLuint mEbo{ 0 };
	GLuint mPosVbo{ 0 };    //位置的VBO
	GLuint mUvVbo{ 0 };     //UV的VBO
	GLuint mNormalVbo{ 0 }; //法线的VBO
	

	uint32_t mIndicesCount{ 0 };
};