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

	1 计算ViewDirection，计算每个像素的世界坐标位置，将相机的的位置以uniform变量的形式传入fs
	  1.1 在vs里面加入了out变量 worldPosition
	  1.2 在fs里面加入了in变量，接纳了worldPosition
	  1.3 在fs里面加入了cameraPosition这种uniform变量
	  1.4 注意：将vs里面的transform这个矩阵改成了modelMatrix
	  1.5 注意：在render里面给cameraPosition进行传输值
	2 计算反射方向 lightReflect
	3 计算lightReflect跟viewDirection之间的夹角cos值(注意：需要处理光线从面的背面照射的情况)
	4 通过对specular进行pow计算，控制光斑的大小
	5 加入specularIntensity来控制高光反射的强度
	6 加入环境光，照亮死黑的地方
*/
//平行光：参数（方向，光强） uniform变量形式
glm::vec3 lightDirection = glm::vec3(-1.0f, 0.0f, -1.0f);
glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);

//specular
float specularIntensity = 0.5f;

//ambient
glm::vec3 ambientColor = glm::vec3(0.2f, 0.2f, 0.2f);

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
	//texture = new Texture("assets/textures/goku.jpg", 0);
	texture = new Texture((std::string(ASSETS_DIR) + "/textures/goku.jpg").c_str(), 0);
}

void prepareCamera() {
	float size = 10.0f;
    camera = new OrthographicCamera(-size, size, size, -size, size, -size);
	//camera = new PerspectiveCamera(
	//	60.0f, 
	//	(float)app->getWidth() / (float)app->getHeight(),
	//	0.1f,
	//	1000.0f
	//);
	cameraControl = new TrackBallCameraControl();
	//cameraControl = new GameCameraControl();
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
	shader->setInt("sampler", 0);
	shader->setMatrix4x4("modelMatrix", transform);                         //模型变换矩阵
	shader->setMatrix4x4("viewMatrix", camera->getViewMatrix());            //视图变换矩阵
	shader->setMatrix4x4("projectionMatrix", camera->getProjectionMatrix());//透视投影矩阵
	
	//光源参数的uniform更新
	shader->setVector3("lightDirection", lightDirection);
	shader->setVector3("lightColor", lightColor);
	shader->setFloat("specularIntensity", specularIntensity);
	shader->setVector3("ambientColor", ambientColor);

	shader->setVector3("cameraPosition", camera->mPosition);

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