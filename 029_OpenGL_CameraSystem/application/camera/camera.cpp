#include "camera.h"

//可以在这里把初始化参数都传进来
Camera::Camera() {}

Camera::~Camera() {}

glm::mat4 Camera::getViewMatrix() {
	glm::vec3 front = glm::cross(mUp, mRight); //mUp与mRight进行叉乘
	glm::vec3 center = mPosition + front;//表示相机始终在front向量上看向世界坐标的点(相机的前向方向)
	//lookat 
    //-eye:相机位置（使用mPosition）
    //-center:看向世界坐标的哪个点
    //-top:穹顶（使用mUp替代）
	return glm::lookAt(mPosition, center, mUp);//返回相机的视图矩阵
}

glm::mat4 Camera::getProjectionMatrix() {
	return glm::identity<glm::mat4>();//父类中返回一个4*4的单位阵，等待子类重写
}

void Camera::scale(float deltaScale) {}



