#include "object.h"

Object::Object() {

}

Object::~Object() {

}

void Object::setPosition(glm::vec3 pos) {
	mPosition = pos;//世界坐标系坐标
}

//增量旋转
void Object::rotateX(float angle) {
	mAngleX += angle;
}

void Object::rotateY(float angle) {
	mAngleY += angle;
}

void Object::rotateZ(float angle) {
	mAngleZ += angle;
}

void Object::setScale(glm::vec3 scale) {
	mScale = scale;
}

glm::mat4 Object::getModelMatrix() {
	//声明一个单位矩阵
	glm::mat4 transform{ 1.0f };              
	//1.进行缩放操作
	transform = glm::scale(transform, mScale);
	//2.进行旋转操作unity旋转标准：pitch yaw roll
	//  2.1 pitch操作,绕x轴
	transform = glm::rotate(transform, glm::radians(mAngleX), glm::vec3(1.0f, 0.0f, 0.0f));
	//  2.2 yaw  操作,绕y轴
	transform = glm::rotate(transform, glm::radians(mAngleY), glm::vec3(0.0f, 1.0f, 0.0f));
	//  2.3 roll 操作,绕z轴
	transform = glm::rotate(transform, glm::radians(mAngleZ), glm::vec3(0.0f, 0.0f, 1.0f));
	//3.进行平移操作(平移矩阵的作用是将物体从局部坐标系原点移动到世界坐标系中的指定位置)
	//  3.1 mPosition是物体在世界坐标系中的位置向量
	//  3.2 glm::mat4(1.0f)：创建一个单位矩阵（相当于不做任何变换）
	//  3.3 glm::translate(单位矩阵, mPosition)：基于单位矩阵创建平移矩阵
	//  3.4 glm::translate(glm::mat4(1.0f),mPosition)优点如下:
	//      3.4.1 保持平移独立性​​,平移在世界坐标系中进行
	//      3.4.2 避免缩放影响​​,平移距离不受缩放影响
	//      3.4.3 方向不变性​​,平移方向始终是世界坐标系方向
	//      3.4.4 符合物理直觉​​,先调整物体自身，再放置到世界位置
	transform = glm::translate(glm::mat4(1.0f),mPosition) * transform;
	//4.返回模型矩阵Modeltrix
	return transform;
}