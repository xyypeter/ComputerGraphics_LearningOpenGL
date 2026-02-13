#pragma once 
#include "core.h"

class Object {
public:
	Object();
	~Object();
	//设置位置,通常任务此处设置的是世界坐标
	void setPosition(glm::vec3 pos);
	//增量旋转(增量变换------+=)
	void rotateX(float angle); 
	void rotateY(float angle); 
	void rotateZ(float angle); 
	//设置缩放因子
	void setScale(glm::vec3 scale);
	//设置模型矩阵
	glm::mat4 getModelMatrix();
	//添加位置的获取函数
	glm::vec3 getPosition() const { return mPosition; }
protected:
	glm::vec3 mPosition{ 0.0f };
	//unity旋转标准：pitch、yaw、roll
	float mAngleX{ 0.0f };   //pitch绕x旋转角度->粗糙理解为上下点头
	float mAngleY{ 0.0f };   //yaw  绕y旋转角度->粗糙理解为左右摇头
	float mAngleZ{ 0.0f };   //roll 绕z旋转角度->粗糙理解为躺在床上滚动,滚筒式旋转
	glm::vec3 mScale{ 1.0f };//缩放因子,默认1.0f,表示没有缩放
};

