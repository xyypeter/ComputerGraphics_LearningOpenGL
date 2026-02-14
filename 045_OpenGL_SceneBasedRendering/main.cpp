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
#include "glframework/material/phongMaterial.h"
#include "glframework/material/whiteMaterial.h"

#include "glframework/mesh.h"
#include "glframework/renderer/renderer.h"
#include "glframework/light/pointLight.h"
#include "glframework/light/spotLight.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "glframework/scene.h"


Renderer* renderer = nullptr;
Scene* scene = nullptr;

//灯光们
DirectionalLight* dirLight = nullptr;

AmbientLight* ambLight = nullptr;

Camera* camera = nullptr;
CameraControl* cameraControl = nullptr;

glm::vec3 clearColor{};

void OnResize(int width, int height) {
	GL_CALL(glViewport(0, 0, width, height));
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

void prepare() {
	renderer = new Renderer();
	scene = new Scene();

	//1 创建geometry
	auto boxGeometry = Geometry::createBox(1.0f);
	auto spGeometry = Geometry::createSphere(1.0f);

	//2 创建一个material并且配置参数
	auto material = new PhongMaterial();
	material->mShiness = 16.0f;
	//material->mDiffuse = new Texture("assets/textures/box.png", 0);
	//material->mSpecularMask = new Texture("assets/textures/sp_mask.png", 1);
	material->mDiffuse = new Texture((std::string(ASSETS_DIR) + "/textures/box.png").c_str(), 0);
	material->mSpecularMask = new Texture((std::string(ASSETS_DIR) + "/textures/sp_mask.png").c_str(), 1);

	//3 生成mesh
	auto mesh = new Mesh(boxGeometry, material);
	auto spMesh01 = new Mesh(spGeometry, material);
	auto spMesh02 = new Mesh(spGeometry, material);
	spMesh01->setPosition(glm::vec3(2.0f, 0.0f, 0.0f));
	spMesh02->setPosition(glm::vec3(-2.0f, 0.0f, 0.0f));

	//创建父子关系
	mesh->addChild(spMesh01);
	mesh->addChild(spMesh02);

	scene->addChild(mesh);
	
	dirLight = new DirectionalLight();
	dirLight->mDirection = glm::vec3(-1.0f);

	ambLight = new AmbientLight();
	ambLight->mColor = glm::vec3(0.1f);
}

void prepareCamera() {
	float size = 10.0f;
	//camera = new OrthographicCamera(-size, size, size, -size, size, -size);
	camera = new PerspectiveCamera(
		60.0f, 
		(float)app->getWidth() / (float)app->getHeight(),
		0.1f,
		1000.0f
	);
	//cameraControl = new GameCameraControl();
	cameraControl = new TrackBallCameraControl();
	cameraControl->setCamera(camera);
	cameraControl->setSensitivity(0.4f);
}

void initIMGUI() {
	ImGui::CreateContext();//创建imgui上下文
	ImGui::StyleColorsDark(); // 选择一个主题

	// 设置ImGui与GLFW和OpenGL的绑定
	ImGui_ImplGlfw_InitForOpenGL(app->getWindow(), true);
	ImGui_ImplOpenGL3_Init("#version 460");
}

void renderIMGUI() {
	//1 开启当前的IMGUI渲染
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	//2 决定当前的GUI上面有哪些控件，从上到下
	ImGui::Begin("Hello, world!");
	ImGui::Text("ChangeColor Demo");
	ImGui::Button("Test Button", ImVec2(40, 20));
	ImGui::ColorEdit3("Clear Color", (float*)&clearColor);
	ImGui::End();

	//3 执行UI渲染
	ImGui::Render();
	//获取当前窗体的宽高
	int display_w, display_h;
	glfwGetFramebufferSize(app->getWindow(), &display_w, &display_h);
	//重置视口大小
	glViewport(0, 0, display_w, display_h);

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
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
	GL_CALL(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));

	prepareCamera();
	prepare();
	initIMGUI();

	while (app->update()) {
		cameraControl->update();
		renderer->setClearColor(clearColor);
		renderer->render(scene, camera,dirLight, ambLight);
		renderIMGUI();
	}

	app->destroy();

	return 0;
}