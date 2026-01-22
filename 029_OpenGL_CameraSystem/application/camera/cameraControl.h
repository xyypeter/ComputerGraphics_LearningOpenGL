#pragma once
#include "../../glframework/core.h"
#include "camera.h"
#include <map>     //stl:map

class CameraControl {
public:
	CameraControl();
	~CameraControl();
	//由于继承CameraControl的子类，有可能会实现自己的逻辑，且多态的实现需要考虑外部调用，子类对象本身的调用可以不用出现在父类中
	virtual void onMouse(int button,int action,double xpos,double ypos);//鼠标按下的响应函数
	virtual void onKey(int key, int action, int mods);                  //键盘按下的响应函数(那个按键操作、操作类型(按下/抬起),状态修饰(alt,ctrl.))
	virtual void onCursor(double xpos, double ypos);                    //鼠标移动的响应函数
	virtual void onScroll(float offset);                                //鼠标滚轮的响应函数(+1 -1)
	virtual void update();                                              //帧渲染,每一帧渲染之前都要进行调用，每一帧更新的行为都可以放在这里
	void setCamera(Camera* camera) { mCamera = camera; }                //设置当前控制的相机
	void setSensitivity(float s) { mSensitivity = s; }                  //设置相机敏感度    
	void setScaleSpeed(float s) { mScaleSpeed = s; }                    //设置相机缩放速度(用于TrackBallCameraControl::onScroll滚轮缩放) 
protected:
	//1 鼠标按键状态
	bool mLeftMouseDown = false;
	bool mRightMouseDown = false;
	bool mMiddleMouseDown = false;
	//2 当前鼠标的位置
	float mCurrentX = 0.0f, mCurrentY = 0.0f;
	//3 记录键盘相关按键的按下状态
	std::map<int, bool> mKeyMap;
	//4 存储当前控制的那一个摄像机
	Camera* mCamera = nullptr;
	//5 敏感度
	float mSensitivity = 0.2f;
	//6 记录相机缩放的速度
	float mScaleSpeed = 0.2f;
};

