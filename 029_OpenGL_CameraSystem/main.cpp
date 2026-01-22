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

/*
1 设计相机数据系统
  1.1 设计相机的ViewMatrix的数据系统
  1.2 需要使用C++的继承与多态实现ProjectionMatrix的计算
2 设计相机控制系统
  2.1 在Application类里面加入对于鼠标，鼠标移动响应
  2.2 设计并且实现CameraControl父类
  2.3 演示CameraControl在主程序中接受事件的方式（在Application里面，增加获取当前鼠标位置的函数）
  2.4 演示CameraControl的update函数调用方式
  2.5 设计实现TrackBallCameraControl的旋转功能+移动功能
	  1 删除原来的投影矩阵跟相机矩阵
	  2 考察那里使用了原来的这些矩阵，做替换
	  3 将原来的测试用的CameraControl替换为Trackball。。。
  2.6 设计实现TrackBallCameraControl的鼠标中键移动相机功能 
  2.7 设计实现TrackBallCameraControl的鼠标滚轮缩放画面 
	  1 在Camera父类中增加scale函数，在两种相机中实现,用于缩放
	  2 在CameraControl父类中增加onScroll，用于响应,子类实现
	  3 在Application中加入鼠标滚动的响应
	  4 将鼠标滚轮响应加入到主流程
  2.8 设计实现GameCameraControl的基础旋转功能
  2.9 设计实现GameCameraControl的漫游移动功能
*/

GLuint vao;
Shader* shader = nullptr;
Texture* texture = nullptr;
glm::mat4 transform(1.0f);

//OrthographicCamera* camera = nullptr;//正交投影相机
PerspectiveCamera* camera = nullptr;   //透视投影相机
 
//TrackBallCameraControl* cameraControl = nullptr;//轨迹球相机
GameCameraControl* cameraControl = nullptr;   //游戏相机

void OnResize(int width, int height) {
	GL_CALL(glViewport(0, 0, width, height));
}

void OnKey(int key, int action, int mods) {
	cameraControl->onKey(key, action, mods);
}

//鼠标按下/抬起
void OnMouse(int button, int action, int mods) {
	double x, y;
	app->getCursorPosition(&x, &y);//获取当前鼠标位置
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
	//1 准备positions colors
	/*float positions[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f,  0.5f, 0.0f,
	};*/
	float positions[] = {
		-1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		0.0f,  1.0f, 0.0f,
	};

	float colors[] = {
		1.0f, 0.0f,0.0f,
		0.0f, 1.0f,0.0f,
		0.0f, 0.0f,1.0f,
	};

	float uvs[] = {
		0.0f, 0.0f,
		1.0f, 0.0f,
		0.5f, 1.0f,
	};

	unsigned int indices[] = {
		0, 1, 2,
	};

	//2 VBO创建
	GLuint posVbo, colorVbo, uvVbo;
	glGenBuffers(1, &posVbo);
	glBindBuffer(GL_ARRAY_BUFFER, posVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);

	glGenBuffers(1, &colorVbo);
	glBindBuffer(GL_ARRAY_BUFFER, colorVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

	glGenBuffers(1, &uvVbo);
	glBindBuffer(GL_ARRAY_BUFFER, uvVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(uvs), uvs, GL_STATIC_DRAW);

	//3 EBO创建
	GLuint ebo;
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//4 VAO创建
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	//5 绑定vbo ebo 加入属性描述信息
	//5.1 加入位置属性描述信息
	glBindBuffer(GL_ARRAY_BUFFER, posVbo);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);

	//5.2 加入颜色属性描述数据
	glBindBuffer(GL_ARRAY_BUFFER, colorVbo);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);

	//5.3 加入uv属性描述数据
	glBindBuffer(GL_ARRAY_BUFFER, uvVbo);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*)0);

	//5.4 加入ebo到当前的vao
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBindVertexArray(0);
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
	// 正交投影矩阵测试
	//float size = 6.0f;
	//camera = new OrthographicCamera(-size, size, size, -size, size, -size);

	// 透视投影矩阵测试
	camera = new PerspectiveCamera(60.0f,(float)app->getWidth() / (float)app->getHeight(),0.1f,1000.0f);
	
	// TrackBallCameraControl控制测试
	//cameraControl = new TrackBallCameraControl();
	//cameraControl->setCamera(camera);
	//cameraControl->setSensitivity(0.4f);

	// GameCameraControl控制测试
	 cameraControl = new GameCameraControl();
	 cameraControl->setCamera(camera);
	 cameraControl->setSensitivity(0.4f);
}

void render() {
	//执行opengl画布清理操作
	GL_CALL(glClear(GL_COLOR_BUFFER_BIT));
	//绑定当前的program
	shader->begin();
	shader->setInt("sampler", 0);
	shader->setMatrix4x4("transform", transform);
	shader->setMatrix4x4("viewMatrix", camera->getViewMatrix());
	shader->setMatrix4x4("projectionMatrix", camera->getProjectionMatrix());
	//绑定当前的vao
	GL_CALL(glBindVertexArray(vao));
	//发出绘制指令
	GL_CALL(glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0));
	GL_CALL(glBindVertexArray(0));
	shader->end();
}

int main() {
	if (!app->init(800, 600)) {
		return -1;
	}
	//消息回调函数部分，相机系统主要关注的是OnKey、OnMouse、OnCursor、OnScroll部分
	app->setResizeCallback(OnResize);
	app->setKeyBoardCallback(OnKey);
	app->setMouseCallback(OnMouse);
	app->setCursorCallback(OnCursor);
	app->setScrollCallback(OnScroll);
	//设置opengl视口以及清理颜色
	GL_CALL(glViewport(0, 0, 800, 600));
	GL_CALL(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));
	prepareShader();
	prepareVAO();
	prepareTexture();
	prepareCamera();
	while (app->update()) {
		cameraControl->update();
		render();
	}
	app->destroy();
	return 0;
}