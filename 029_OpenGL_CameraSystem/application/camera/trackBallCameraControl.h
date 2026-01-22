#pragma once

#include "cameraControl.h"

class TrackBallCameraControl :public CameraControl {
public:
	TrackBallCameraControl();
	~TrackBallCameraControl();
	void onCursor(double xpos, double ypos) override; //鼠标移动的响应函数
	void onScroll(float offset) override;             //鼠标滚轮的响应函数(缩放因子)
private:
	//pitch状态下，相机上下旋转，mPosition,mUp发生改变，mRight没有发生改变
	//因为Pitch转动轴就是当前相机的X轴，也就是right向量，所以没有发生改变
	void pitch(float angle);
	//yaw状态下，相机左右旋转，mPosition,mUp,mRight均发生改变
	void yaw(float angle);  
private:
	float mMoveSpeed = 0.005f;//偏移量系数
};







