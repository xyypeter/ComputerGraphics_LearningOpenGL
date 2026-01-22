#include "gameCameraControl.h"

GameCameraControl::GameCameraControl() {}

GameCameraControl::~GameCameraControl() {}

void GameCameraControl::pitch(float angle) {
	//需要增加此部分，否则无限制抬头和无限制低头，不符合现实使用场景
	mPitch += angle;
	if (mPitch > 89.0f || mPitch < -89.0f) {
		mPitch -= angle;//不符合条件需要撤回累加
		return;
	}
    //在gameCameraControl的情况下，pitch不会影响mPosition
	//绕着mRight向量旋转，三项参数分别为 单位矩阵,旋转角度,旋转轴
	auto mat = glm::rotate(glm::mat4(1.0f), glm::radians(angle), mCamera->mRight);
	mCamera->mUp = mat * glm::vec4(mCamera->mUp, 0.0f);
}

void GameCameraControl::yaw(float angle) {
	//绕着世界坐标系的Y轴旋转
	auto mat = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0.0f,1.0f,0.0f));
	//游戏相机中pitch不会影响mPosition(位置)，会影响mUp和mRight
	mCamera->mUp = mat * glm::vec4(mCamera->mUp, 0.0f);
	mCamera->mRight = mat * glm::vec4(mCamera->mRight, 0.0f);
}

void GameCameraControl::onCursor(double xpos, double ypos) {
	float deltaX = (xpos - mCurrentX) * mSensitivity;//mSensitivity表示敏感度
	float deltaY = (ypos - mCurrentY) * mSensitivity;
	//跟随鼠标右键旋转的操作部分
	if (mRightMouseDown) {
		pitch(-deltaY);//添加负号改变鼠标移动的方向，若不添加负号，鼠标上移，内容上移(根据需求或者个人习惯调整)
		yaw(-deltaX);
	}
	mCurrentX = xpos;
	mCurrentY = ypos;
}

void GameCameraControl::update() {
	//由于父类中mKeyMap，已经记录了键盘相关按键的按下状态，又因为每一帧需要检测WASD修改mPosition,所以直接重写父类的update()
	glm::vec3 direction(0.0f);                             //创建最终移动方向变量
	auto front = glm::cross(mCamera->mUp, mCamera->mRight);//mUp和mRight叉乘，得到front向量
	auto right = mCamera->mRight;
	if (mKeyMap[GLFW_KEY_W]) {
		direction += front;
	}
	if (mKeyMap[GLFW_KEY_S]) {
		direction -= front;
	}
	if (mKeyMap[GLFW_KEY_A]) {
		direction -= right;
	}
	if (mKeyMap[GLFW_KEY_D]) {
		direction += right;
	}
	//此时direction有可能不为1的长度，也有可能是0的长度，归一化的操作
	if (glm::length(direction) != 0) {
		direction = glm::normalize(direction);   //不等于0的情况下进行，归一化操作，得到纯粹的移动方向
		mCamera->mPosition += direction * mSpeed;//纯粹的移动方向*移动速度，得到本帧移动的单位长度
	}
}