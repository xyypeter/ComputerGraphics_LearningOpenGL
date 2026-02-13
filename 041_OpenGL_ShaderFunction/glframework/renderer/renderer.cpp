#include "renderer.h"
#include <iostream>
#include "../material/phongMaterial.h"
#include "../material/whiteMaterial.h"

Renderer::Renderer() {
	//mPhongShader = new Shader("assets/shaders/phong.vert", "assets/shaders/phong.frag");
	mPhongShader = new Shader((std::string(ASSETS_DIR) + "/shaders/phong.vert").c_str(), (std::string(ASSETS_DIR) + "/shaders/phong.frag").c_str());
	//mWhiteShader = new Shader("assets/shaders/white.vert", "assets/shaders/white.frag");
	mWhiteShader = new Shader((std::string(ASSETS_DIR) + "/shaders/white.vert").c_str(), (std::string(ASSETS_DIR) + "/shaders/white.frag").c_str());
}

Renderer::~Renderer() {

}

void Renderer::render(
	const std::vector<Mesh*>& meshes,
	Camera* camera,
	DirectionalLight* dirLight,
	PointLight* pointLight,
	SpotLight* spotLight,
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
			//spotlight的更新
			shader->setVector3("spotLight.position", spotLight->getPosition());
			shader->setVector3("spotLight.color", spotLight->mColor);
			shader->setVector3("spotLight.targetDirection", spotLight->mTargetDirection);
			shader->setFloat("spotLight.specularIntensity", spotLight->mSpecularIntensity);
			shader->setFloat("spotLight.innerLine", glm::cos(glm::radians(spotLight->mInnerAngle)));
			shader->setFloat("spotLight.outerLine", glm::cos(glm::radians(spotLight->mOuterAngle)));
 
			//directionalLight 的更新
			shader->setVector3("directionalLight.color", dirLight->mColor);
			shader->setVector3("directionalLight.direction", dirLight->mDirection);
			shader->setFloat("directionalLight.specularIntensity", dirLight->mSpecularIntensity);

			//pointLight的更新
			shader->setVector3("pointLight.color", pointLight->mColor);
			shader->setVector3("pointLight.position", pointLight->getPosition());
			shader->setFloat("pointLight.specularIntensity", pointLight->mSpecularIntensity);
			shader->setFloat("pointLight.k2", pointLight->mK2);
			shader->setFloat("pointLight.k1", pointLight->mK1);
			shader->setFloat("pointLight.kc", pointLight->mKc);


			shader->setFloat("shiness", phongMat->mShiness);
			shader->setVector3("ambientColor", ambLight->mColor);
			//相机信息更新
			shader->setVector3("cameraPosition", camera->mPosition);
		}	
			break;
		case MaterialType::WhiteMaterial: {
			//mvp
			shader->setMatrix4x4("modelMatrix", mesh->getModelMatrix());
			shader->setMatrix4x4("viewMatrix", camera->getViewMatrix());
			shader->setMatrix4x4("projectionMatrix", camera->getProjectionMatrix());
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
	case MaterialType::WhiteMaterial:
		result = mWhiteShader;
		break;
	default:
		std::cout << "Unknown material type to pick shader" << std::endl;
		break;
	}

	return result;
}
