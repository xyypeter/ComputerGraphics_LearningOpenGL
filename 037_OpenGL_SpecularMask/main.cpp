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
#include "glframework/mesh.h"
#include "glframework/renderer/renderer.h"


//1 在材质类中，加入SpecularMask纹理对象作为蒙版，纹理单元为1box
//2 在FragmentShader中加入SpecularMask纹理的采样器
//3 将SpecularMask加入到帧更新步骤中
//4 在FS中，使用当前的uv对SpecularMask进行采样，取其r通道作为高光的比例 

Renderer* renderer = nullptr;
std::vector<Mesh*> meshes{};
DirectionalLight* dirLight = nullptr;
AmbientLight* ambLight = nullptr;

Camera* camera = nullptr;
GameCameraControl* cameraControl = nullptr;


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
	//renderer = new Renderer();

	////1 创建geometry
	//auto geometry = Geometry::createBox(1.5f);

	////2 创建一个material并且配置参数
	//auto material = new PhongMaterial();
	//material->mShiness = 16.0f;
	////material->mDiffuse = new Texture("assets/textures/box.png", 0);
	//material->mDiffuse = new Texture((std::string(ASSETS_DIR) + "/textures/box.png").c_str(), 0);
	////material->mSpecularMask = new Texture("assets/textures/sp_mask.png", 1);
	//material->mSpecularMask = new Texture((std::string(ASSETS_DIR) + "/textures/sp_mask.png").c_str(), 1);

	////3 生成mesh
	//auto mesh = new Mesh(geometry, material);

	//meshes.push_back(mesh);

	//dirLight = new DirectionalLight();
	//dirLight->mDirection = glm::vec3(-1.0, 0.0, -1.0);
	//ambLight = new AmbientLight();
	//ambLight->mColor = glm::vec3(0.1f);
		//1 准备渲染器
	renderer = new Renderer();
	//2 创建几何体geometry
	auto geometry = Geometry::createBox(1.5f);     //立方体
	//2 创建一个材质material并且配置参数
	auto material1 = new PhongMaterial();                            //创建冯氏材质
	material1->mShiness = 1.0f;                                      //光斑影响因子
	//material1->mDiffuse = new Texture("assets/textures/box.png", 0);//diffuse贴图
	material1->mDiffuse = new Texture((std::string(ASSETS_DIR) + "/textures/box.png").c_str(), 0);
	//material1->mSpecularMask = new Texture("assets/textures/sp_mask.png", 1);
	material1->mSpecularMask = new Texture((std::string(ASSETS_DIR) + "/textures/sp_mask.png").c_str(), 1);
	//3 生成mesh网格体
	auto mesh1 = new Mesh(geometry, material1);                       //生成Mesh1
	meshes.push_back(mesh1);
	dirLight = new DirectionalLight();
	dirLight->mDirection = glm::vec3(-1.0, 0.0, -1.0);
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

	cameraControl = new GameCameraControl();
	cameraControl->setCamera(camera);
	cameraControl->setSensitivity(0.4f);
	cameraControl->setSpeed(0.03f);
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

	prepareCamera();
	prepare();

	while (app->update()) {
		cameraControl->update();
		renderer->render(meshes, camera, dirLight, ambLight);
	}

	app->destroy();

	return 0;
}