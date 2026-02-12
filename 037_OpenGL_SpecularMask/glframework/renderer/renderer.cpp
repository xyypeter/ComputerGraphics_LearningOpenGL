#include "renderer.h"
#include <iostream>
#include "../material/phongMaterial.h"

Renderer::Renderer() {
	//mPhongShader = new Shader("assets/shaders/phong.vert", "assets/shaders/phong.frag");
	mPhongShader = new Shader((std::string(ASSETS_DIR) + "/shaders/phong.vert").c_str(), (std::string(ASSETS_DIR) + "/shaders/phong.frag").c_str());
}

Renderer::~Renderer() {

}

void Renderer::render(
	const std::vector<Mesh*>& meshes,
	Camera* camera,
	DirectionalLight* dirLight,
	AmbientLight* ambLight
) {
	//1 设置当前帧绘制的时候，opengl的必要状态机参数
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	//2 清理画布 
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//3 遍历mesh进行绘制
	for (int i = 0; i < meshes.size(); i++) {
		auto mesh = meshes[i];
		auto geometry = mesh->mGeometry;
		auto material = mesh->mMaterial;
 
		//1 决定使用哪个Shader 
		Shader* shader = pickShader(material->mType);

		//2 更新shader的uniform
		shader->begin();

		switch (material->mType) {
		case MaterialType::PhongMaterial: {
			PhongMaterial* phongMat = (PhongMaterial*)material;

			//diffuse贴图帧更新
			//将纹理采样器与纹理单元进行挂钩
			shader->setInt("sampler", 0);
			//将纹理与纹理单元进行挂钩
			phongMat->mDiffuse->bind();

			//高光蒙版的帧更新
			shader->setInt("specularMaskSampler", 1);
			phongMat->mSpecularMask->bind();

			//mvp
			shader->setMatrix4x4("modelMatrix", mesh->getModelMatrix());
			shader->setMatrix4x4("viewMatrix", camera->getViewMatrix());
			shader->setMatrix4x4("projectionMatrix", camera->getProjectionMatrix());

			auto normalMatrix = glm::mat3(glm::transpose(glm::inverse(mesh->getModelMatrix())));
			shader->setMatrix3x3("normalMatrix", normalMatrix);

			//光源参数的uniform更新
			shader->setVector3("lightDirection", dirLight->mDirection);
			shader->setVector3("lightColor", dirLight->mColor);
			shader->setFloat("specularIntensity", dirLight->mSpecularIntensity);

			shader->setFloat("shiness", phongMat->mShiness);

			shader->setVector3("ambientColor", ambLight->mColor);

			//相机信息更新
			shader->setVector3("cameraPosition", camera->mPosition);

		}	
			break;
		default:
			continue;
		}
	
		//3 绑定vao
		glBindVertexArray(geometry->getVao());

		//4 执行绘制命令
		glDrawElements(GL_TRIANGLES, geometry->getIndicesCount(), GL_UNSIGNED_INT, 0);
	}
}

Shader* Renderer::pickShader(MaterialType type) {
	Shader* result = nullptr;

	switch (type) {
	case MaterialType::PhongMaterial:
		result = mPhongShader;
		break;
	default:
		std::cout << "Unknown material type to pick shader" << std::endl;
		break;
	}

	return result;
}
