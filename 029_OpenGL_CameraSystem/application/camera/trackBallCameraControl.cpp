#include "trackBallCameraControl.h"


TrackBallCameraControl::TrackBallCameraControl() {}

TrackBallCameraControl::~TrackBallCameraControl() {}

void TrackBallCameraControl::onCursor(double xpos, double ypos) {
	if (mLeftMouseDown) {
		// 1 点住左键，开始调整相机的各类参数，计算经线跟纬线旋转的增量角度(正负都有可能）
		float deltaX = (xpos - mCurrentX) * mSensitivity;//X的变化量*敏感度
		float deltaY = (ypos - mCurrentY) * mSensitivity;//Y的变化量*敏感度
		// 2 分开pitch和yaw各自计算，下面使用负号的含义是，与鼠标点击后旋转的方向有关系(鼠标向左内容将向左，其余同理)
		pitch(-deltaY);
		yaw(-deltaX);  
	}else if (mMiddleMouseDown) {
		float deltaX = (xpos - mCurrentX) * mMoveSpeed;//(正负都有可能）
		float deltaY = (ypos - mCurrentY) * mMoveSpeed;//(正负都有可能）
		mCamera->mPosition += mCamera->mUp * deltaY;
		mCamera->mPosition -= mCamera->mRight * deltaX;//负号影响的依然是方向
	}
	mCurrentX = xpos;
	mCurrentY = ypos;
}

void TrackBallCameraControl::pitch(float angle) {
	// 绕着mRight向量在旋转，三项参数分别为，单位矩阵，旋转角度，旋转轴
	auto mat = glm::rotate(glm::mat4(1.0f), glm::radians(angle), mCamera->mRight);
	// 影响当前相机的up向量和位置Position
	mCamera->mUp = mat * glm::vec4(mCamera->mUp, 0.0f);//错误编写方式mat * mCamera->mUp;因为mat是4*4的矩阵，然而mCamera->mUp是一个三维向量
	mCamera->mPosition = mat * glm::vec4(mCamera->mPosition, 1.0f);
}

void TrackBallCameraControl::yaw(float angle) {
	// 绕着世界坐标系的y轴旋转
	auto mat = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0.0f,1.0f,0.0f));
	mCamera->mUp = mat * glm::vec4(mCamera->mUp, 0.0f);
	mCamera->mRight = mat * glm::vec4(mCamera->mRight, 0.0f);
	mCamera->mPosition = mat * glm::vec4(mCamera->mPosition, 1.0f);
}

void TrackBallCameraControl::onScroll(float offset) {
	mCamera->scale(mScaleSpeed * offset);
}