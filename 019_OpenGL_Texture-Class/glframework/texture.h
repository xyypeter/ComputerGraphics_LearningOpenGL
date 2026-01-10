#pragma once
#include "core.h"
#include <string>

class Texture {
public:
	Texture(const std::string& path, unsigned int unit);
	~Texture();
	void bind();
private:
	GLuint mTexture{ 0 };   //创建一个纹理对象时，glGenTextures 函数会返回一个纹理 ID 
	unsigned int mUnit{ 0 };//绑在那里纹理单元上
	int mWidth{ 0 };        //宽 
	int mHeight{ 0 };       //高
};