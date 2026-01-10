#include "texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../application/stb_image.h"

Texture::Texture(const std::string& path, unsigned int unit) {
	mUnit = unit;
	//1 stbImage 读取图片
	int channels;
	//--反转y轴
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(path.c_str(), &mWidth, &mHeight, &channels, STBI_rgb_alpha);
	//2 生成纹理并且激活单元绑定
	glGenTextures(1, &mTexture);
	//--激活纹理单元--GL_TEXTURE0 是第一个纹理单元的常量，通常表示 texture unit 0
	glActiveTexture(GL_TEXTURE0 + mUnit);
	//--绑定纹理对象--
	glBindTexture(GL_TEXTURE_2D, mTexture);
	//3 传输纹理数据,开辟显存
	//void glTexImage2D(
	//	GLenum target,    // 纹理目标
	//	GLint level,      // 纹理的级别（通常是0，表示基础级别）
	//	GLint internalformat,  // 内部格式：GPU内部存储的纹理数据格式
	//	GLsizei width,    // 纹理的宽度
	//	GLsizei height,   // 纹理的高度
	//	GLint border,     // 边框宽度，通常是0
	//	GLenum format,    // 输入数据的格式
	//	GLenum type,      // 输入数据的类型
	//	const void* data  // 纹理数据的指针
	//);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mWidth, mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	//***释放数据
	stbi_image_free(data);
	//4 设置纹理的过滤方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//5 设置纹理的包裹方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//u
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);//v
}

Texture::~Texture() {
	if (mTexture != 0) {
		glDeleteTextures(1, &mTexture);
	}
}

//bind() 函数通过选择纹理单元并将纹理对象绑定到该单元，使得渲染管线能够使用该纹理进行纹理采样
void Texture::bind() {
	//glActiveTexture 用来激活一个纹理单元
	//GL_TEXTURE0 是第一个纹理单元，GL_TEXTURE1 是第二个纹理单元，通过 GL_TEXTURE0 + mUnit，你可以动态选择纹理单元
	glActiveTexture(GL_TEXTURE0 + mUnit);
	//用来绑定一个具体的纹理到当前活动的纹理单元上
	glBindTexture(GL_TEXTURE_2D, mTexture);
}