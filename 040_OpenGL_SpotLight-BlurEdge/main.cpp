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
//引入几何体类+冯氏材质+Mesh网格+渲染器Renderer
#include "glframework/geometry.h"
#include "glframework/material/phongMaterial.h"
#include "glframework/material/whiteMaterial.h"
#include "glframework/mesh.h"
#include "glframework/renderer/renderer.h"
#include "glframework/light/pointLight.h"
#include "glframework/light/spotLight.h"

Renderer* renderer = nullptr;          //渲染器
std::vector<Mesh*> meshes{};           //Mesh网格体集合
//DirectionalLight* dirLight = nullptr;  //平行光
//PointLight* pointLight = nullptr;      //点光源
SpotLight* spotLight = nullptr;        //探照灯光源
Mesh* meshWhite;
AmbientLight* ambLight = nullptr;      //环境光
Camera* camera = nullptr;              //相机类(透视投影相机,正交投影相机)
CameraControl* cameraControl = nullptr;//相机控制类(游戏控制,轨迹球相机控制)

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
	//1 准备渲染器
	renderer = new Renderer();
	//2 创建几何体geometry
	auto geometry = Geometry::createBox(2.0f);     //立方体
	//2 创建一个材质material并且配置参数
	auto material1 = new PhongMaterial();                            //创建冯氏材质
	material1->mShiness = 16.0f;                                      //光斑影响因子
	//material1->mDiffuse = new Texture("assets/textures/box.png", 0);//diffuse贴图
	material1->mDiffuse = new Texture((std::string(ASSETS_DIR) + "/textures/box.png").c_str(), 0);

	//material1->mSpecularMask=new Texture("assets/textures/sp_mask.png", 1);
	material1->mSpecularMask = new Texture((std::string(ASSETS_DIR) + "/textures/sp_mask.png").c_str(), 1);
	//3 生成mesh网格体
	auto mesh1 = new Mesh(geometry, material1);                       //生成Mesh1
	meshes.push_back(mesh1);
	//4 创建白色物体
	auto geometryWhite = Geometry::createSphere(0.1f);
	auto materialWhite = new WhiteMaterial();
	meshWhite     =new Mesh(geometryWhite, materialWhite);
	meshWhite->setPosition(glm::vec3(2.0,0,0));
	meshes.push_back(meshWhite);

	spotLight = new SpotLight();
	spotLight->setPosition(meshWhite->getPosition());
	spotLight->mTargetDirection = glm::vec3(-1.0f, 0.0f, 0.0f);
	spotLight->mInnerAngle = 30.0f;
	spotLight->mOuterAngle = 45.0f;
	ambLight = new AmbientLight();
	ambLight->mColor = glm::vec3(0.1f);
}

void prepareCamera() {
	float size = 10.0f;
	//camera = new OrthographicCamera(-size, size, size, -size, size, -size);//正交投影相机
	camera = new PerspectiveCamera(
		60.0f, 
		(float)app->getWidth() / (float)app->getHeight(),
		0.1f,
		1000.0f
	);
	//相机控制类
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
	GL_CALL(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));
	prepareCamera();//准备相机
	prepare();      //准备工作
	while (app->update()) {
		cameraControl->update();
		renderer->renderSpotLight(meshes, camera, spotLight, ambLight);
	}
	app->destroy();
	return 0;
}