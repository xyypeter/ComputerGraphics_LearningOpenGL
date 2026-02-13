#pragma once
#include <vector>
#include "../core.h"
#include "../mesh.h"
#include "../../application/camera/camera.h"
#include "../light/directionalLight.h"
#include "../light/pointLight.h"
#include "../light/spotLight.h"
#include "../light/ambientLight.h"
#include "../shader.h"

class Renderer {
public:
	Renderer();
	~Renderer();
	//渲染函数接收平行光
	void render(const std::vector<Mesh*>& meshes,Camera* camera,DirectionalLight* dirLight,AmbientLight* ambLight);
	//渲染函数接收点光源
	void renderPointLight(const std::vector<Mesh*>& meshes, Camera* camera, PointLight* pointLight, AmbientLight* ambLight);
	//渲染函数接收探照灯光源
	void renderSpotLight(const std::vector<Mesh*>& meshes, Camera* camera, SpotLight* spotLight, AmbientLight* ambLight);
private:
	//根据Material类型不同，挑选不同的shader
	Shader* pickShader(MaterialType type);
private:
	//生成多种不同的shader对象，根据材质类型的不同，挑选使用哪一个shader对象
	Shader* mPhongShader{ nullptr };//冯氏
	Shader* mWhiteShader{ nullptr };//纯白
};