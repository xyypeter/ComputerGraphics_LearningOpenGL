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


Renderer* renderer = nullptr;
std::vector<Mesh*> meshes{};

Mesh* meshWhite = nullptr;

//灯光们
DirectionalLight* dirLight = nullptr;
SpotLight* spotLight = nullptr;
PointLight* pointLight = nullptr;


AmbientLight* ambLight = nullptr;

Camera* camera = nullptr;
CameraControl* cameraControl = nullptr;


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

	//1 创建geometry
	auto geometry = Geometry::createBox(2.5f);

	//2 创建一个material并且配置参数
	auto material = new PhongMaterial();
	material->mShiness = 16.0f;
	//material1->mDiffuse = new Texture("assets/textures/box.png", 0);//diffuse贴图
	material->mDiffuse = new Texture((std::string(ASSETS_DIR) + "/textures/box.png").c_str(), 0);

	//material1->mSpecularMask=new Texture("assets/textures/sp_mask.png", 1);
	material->mSpecularMask = new Texture((std::string(ASSETS_DIR) + "/textures/sp_mask.png").c_str(), 1);
	

	//3 生成mesh
	auto mesh = new Mesh(geometry, material);

	meshes.push_back(mesh);


	//创建白色物体
	auto geometryWhite = Geometry::createSphere(0.1f);
	auto materialWhite = new WhiteMaterial();
	meshWhite = new Mesh(geometryWhite, materialWhite);
	meshWhite->setPosition(glm::vec3(2.0, 0.0, 0.0));

	meshes.push_back(meshWhite);

	spotLight = new SpotLight();
	spotLight->setPosition(meshWhite->getPosition());
	spotLight->mTargetDirection = glm::vec3(-1.0f,0.0f,0.0f);
	spotLight->mInnerAngle = 30.0f;
	spotLight->mOuterAngle = 45.0f;

	dirLight = new DirectionalLight();
	dirLight->mDirection = glm::vec3(1.0f);

	pointLight = new PointLight();
	pointLight->setPosition(glm::vec3(0.0f,0.0f,1.5f));
	pointLight->mSpecularIntensity = 0.5f;
	pointLight->mK2 = 0.017f;
	pointLight->mK1 = 0.07f;
	pointLight->mK2 = 1.0f;

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

	while (app->update()) {
		cameraControl->update();
		renderer->render(meshes, camera,dirLight,pointLight, spotLight, ambLight);
	}

	app->destroy();

	return 0;
}