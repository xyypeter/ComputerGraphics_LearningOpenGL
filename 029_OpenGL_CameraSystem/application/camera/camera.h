#pragma once

#include "../../glframework/core.h"

//使用../返回上一级文件夹
class Camera {
public:
	Camera();
	~Camera();
	// 获取相机的视图矩阵，视图矩阵表示从相机位置看向场景的变换
	glm::mat4 getViewMatrix();
	// 获取相机的投影矩阵，投影矩阵通常用于将场景中的3D物体映射到2D屏幕上
	virtual glm::mat4 getProjectionMatrix();
	// 缩放功能，改变相机的缩放因子（一般用于改变视野的大小）
	virtual void scale(float deltaScale);
public:
	// 相机的位置，通常表示相机在世界空间中的位置，默认为 (0.0f, 0.0f, 5.0f)
	glm::vec3 mPosition{ 0.0f,0.0f,5.0f };
	// 相机的“上”方向（通常是Y轴方向），决定了相机的上下方向，默认为 (0.0f, 1.0f, 0.0f)  {表示脑袋上方而不是天空}
	glm::vec3 mUp{ 0.0f, 1.0f, 0.0f };
	// 相机的“右”方向（通常是X轴方向），决定了相机的左右方向，默认为 (1.0f, 0.0f, 0.0f)
	glm::vec3 mRight{ 1.0f,0.0f,0.0f };
};




