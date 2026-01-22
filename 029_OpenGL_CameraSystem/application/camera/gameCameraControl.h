#pragma once
#include "cameraControl.h"

class GameCameraControl :public CameraControl {
public:
	GameCameraControl();
	~GameCameraControl();
	void onCursor(double xpos, double ypos)override; //鼠标移动的响应函数
	void update()override;                           //帧渲染
	void setSpeed(float s) { mSpeed = s; }           //设置移动速度
private:
	//pitch状态下，相机上下点头，且在游戏相机中pitch不会影响mPosition(位置)
	void pitch(float angle);
	//yaw状态下，相机左右旋转，且在游戏相机中pitch不会影响mPosition(位置)
	void yaw(float angle);
private:
	float mPitch{ 0.0f };  //每帧累加计算，保留Pitch角度
	float mSpeed{ 0.001f };//移动速度
};