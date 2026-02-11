#include <iostream>

#include "glframework/core.h"
#include "glframework/shader.h"
#include <string>
#include <assert.h>//断言
#include "wrapper/checkError.h"
#include "application/Application.h"
#include "glframework/texture.h"

//引入相机+控制器
#include "application/camera/perspectiveCamera.h"
#include "application/camera/orthographicCamera.h"
#include "application/camera/trackBallCameraControl.h"
#include "application/camera/GameCameraControl.h"

#include "glframework/geometry.h"

/*
	1.为Geometry类中的box sphere以及plane加入法线属性
	  1.1 验证normal的正确加入（将normal作为颜色输出）
	2.设置光源的参数，加入到shader中，并且验证
	  2.1 设置光源参数，在C++中
	  2.2 将光源参数传输到shader中，以uniform变量形式
	3.将diffuse（漫反射）加入到fragmentshader当中
*/

//平行光：参数（方向，光强） uniform变量形式
glm::vec3 lightDirection = glm::vec3(-0.4f, -1.4, -1.9f);
glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);

Geometry* geometry = nullptr;
Shader* shader = nullptr;
Texture* texture = nullptr;
glm::mat4 transform(1.0f);

Camera* camera = nullptr;
CameraControl* cameraControl = nullptr;

void OnResize(int width, int height) {
	GL_CALL(glViewport(0, 0, width, height));
	std::cout << "OnResize" << std::endl;
}

void OnKey(int key, int action, int mods) {
	cameraControl->onKey(key, action, mods);
}

//鼠标按下/抬起
void OnMouse(int button, int action, int mods) {
	double x, y;
	app->getCursorPosition(&x, &y);
	cameraControl->onMouse(button, action, x, y);
}

//鼠标移动
void OnCursor(double xpos, double ypos) {
	cameraControl->onCursor(xpos, ypos);
}

//鼠标滚轮
void OnScroll(double offset) {
	cameraControl->onScroll(offset);
}

void prepareVAO() {
	geometry = Geometry::createBox(3.0f);
}

void prepareShader() {
	//shader = new Shader("assets/shaders/vertex.glsl","assets/shaders/fragment.glsl");
	shader = new Shader((std::string(ASSETS_DIR) + "/shaders/vertex.glsl").c_str(), (std::string(ASSETS_DIR) + "/shaders/fragment.glsl").c_str());
}

void prepareTexture() {
	//texture = new Texture("assets/textures/goku.jpg", 0);//将纹理绑定到纹理单元 GL_TEXTURE0
	texture = new Texture((std::string(ASSETS_DIR) + "/textures/goku.jpg").c_str(), 0);
}

void prepareCamera() {	
	//正交投影相机
	//float size = 10.0f;
	//camera = new OrthographicCamera(-size, size, size, -size, size, -size);
	
	//透视投影相机
	camera = new PerspectiveCamera(60.0f,(float)app->getWidth() / (float)app->getHeight(),0.1f,1000.0f);
	
	//游戏相机控制器
	//cameraControl = new GameCameraControl();
	
	//轨迹球相机控制器
	cameraControl = new TrackBallCameraControl();
	cameraControl->setCamera(camera);
	cameraControl->setSensitivity(0.4f);
}

void prepareState() {
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
}

void render() {
	//执行opengl画布清理操作
	GL_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

	//绑定当前的program
	shader->begin();
	//把值 0 赋给着色器中的 uniform 变量 sampler，也就是说：这个采样器从纹理单元 GL_TEXTURE0 采样
	//texture = new Texture("assets/textures/goku.jpg", 0);//绑定到 GL_TEXTURE0
	shader->setInt("sampler", 0); //通知着色器 sampler 对应 GL_TEXTURE0
	shader->setMatrix4x4("transform", transform);
	shader->setMatrix4x4("viewMatrix", camera->getViewMatrix());
	shader->setMatrix4x4("projectionMatrix", camera->getProjectionMatrix());
	
	//光源参数的uniform更新
	shader->setVector3("lightDirection", lightDirection);
	shader->setVector3("lightColor", lightColor);
	//绑定当前的vao
	GL_CALL(glBindVertexArray(geometry->getVao()));
	//发出绘制指令
	GL_CALL(glDrawElements(GL_TRIANGLES, geometry->getIndicesCount(), GL_UNSIGNED_INT, 0));
	GL_CALL(glBindVertexArray(0));
	shader->end();
}

int main() {
	if (!app->init(1600, 1200)) {
		return -1;
	}
	app->setResizeCallback(OnResize);
	app->setKeyBoardCallback(OnKey);
	app->setMouseCallback(OnMouse);
	app->setCursorCallback(OnCursor);
	app->setScrollCallback(OnScroll);
	//设置opengl视口以及清理颜色
	GL_CALL(glViewport(0, 0, 1600, 1200));
	GL_CALL(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));
    //测试部分
	prepareShader();
	prepareVAO();
	prepareTexture();
	prepareCamera();
	prepareState();

	while (app->update()) {
		cameraControl->update();
		render();
	}
	app->destroy();
	return 0;
}