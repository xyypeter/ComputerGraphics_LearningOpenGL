#pragma once 
#include "core.h"

class Object {
public:
	Object();
	~Object();
	
	void setPosition(glm::vec3 pos);

	//增量旋转
	void rotateX(float angle); 
	void rotateY(float angle); 
	void rotateZ(float angle); 

	void setScale(glm::vec3 scale);

	glm::vec3 getPosition()const { return mPosition; }

	glm::mat4 getModelMatrix();

	//父子关系
	void addChild(Object* obj);          //添加孩子至当前对象
	std::vector<Object*>  getChildren(); //获取孩子
	Object* getParent();                 //获取父亲

protected:
	glm::vec3 mPosition{ 0.0f };
	
	//unity旋转标准：pitch yaw roll
	float mAngleX{ 0.0f };
	float mAngleY{ 0.0f };
	float mAngleZ{ 0.0f };

	glm::vec3 mScale{ 1.0f };

	//父子关系
	std::vector<Object*>	mChildren{};       //孩子
	Object*					mParent{ nullptr };//父亲
};