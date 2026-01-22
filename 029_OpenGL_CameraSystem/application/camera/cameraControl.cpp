#include "cameraControl.h"
#include <iostream>

CameraControl::CameraControl() {}

CameraControl::~CameraControl() {}

void CameraControl::onMouse(int button, int action, double xpos, double ypos) {
	//1 判断当前的按键，是否按下
	bool pressed = action == GLFW_PRESS ? true : false;
	//2 如果按下，记录当前按下的位置
	if (pressed) {
		mCurrentX = xpos;
		mCurrentY = ypos;
	}
	//3 根据按下的鼠标按键不同，更改按键状态
	switch (button) {
	case GLFW_MOUSE_BUTTON_LEFT:
		mLeftMouseDown = pressed;
		break;
	case GLFW_MOUSE_BUTTON_RIGHT:
		mRightMouseDown = pressed;
		break;
	case GLFW_MOUSE_BUTTON_MIDDLE:
		mMiddleMouseDown = pressed;
		break;
	}
}

void CameraControl::onKey(int key, int action, int mods) {
	//举例说明此函数:
	// 键（Key）：int 类型，对应 GLFW 的键盘按键代号（如 GLFW_KEY_W、GLFW_KEY_A 等）
	// 值（Value）：bool 类型，表示按键的当前状态:true：按键被按下（GLFW_PRESS）和 false：按键被释放（GLFW_RELEASE）
	//当W被按下时:
	// 1.key = GLFW_KEY_W，action = GLFW_PRESS,pressed = true
	// 2.mKeyMap[GLFW_KEY_W] = true（记录 W 键被按下）
	//当w被释放时: 
	// 1.key = GLFW_KEY_W，action = GLFW_RELEASE,pressed = false
	// 2.mKeyMap[GLFW_KEY_W] = false（记录 W 键已释放）
	
	//过滤掉repeat的情况，过滤按键重复事件（如长按产生的连续触发）
	if (action == GLFW_REPEAT) {
		return;
	}
	//检测按下或者抬起，给到一个变量
	bool pressed = action == GLFW_PRESS ? true : false;
	//记录在keyMap
	mKeyMap[key] = pressed;
}

//游戏相机和轨迹球相机逻辑不同，由子类中重写吧
void CameraControl::onCursor(double xpos, double ypos) {}

//只需要TrackBallCameraControl进行重写，GameCameraControl不需要
void CameraControl::onScroll(float offset) {}

void CameraControl::update() {}

