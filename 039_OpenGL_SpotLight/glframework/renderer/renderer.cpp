#include "renderer.h"
#include "renderer.h"
#include <iostream>
#include "../material/phongMaterial.h"
#include "../material/whiteMaterial.h"

Renderer::Renderer() {
	// 创建冯氏着色器对象
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
	AmbientLight* ambLight
) {
	//1 设置当前帧绘制的时候，opengl的必要状态机参数(深度检测->防止物体出现前后遮挡)
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
		//2 更新shader的uniform(激活当前shader着色器程序​​，使其成为OpenGL渲染管线中当前使用的着色器)
		shader->begin();
		switch (material->mType) {
		case MaterialType::PhongMaterial: {
			PhongMaterial* phongMat = (PhongMaterial*)material;
			//diffuse贴图,设置采样器使用纹理单元0
			shader->setInt("sampler", 0);
			//将纹理与纹理单元进行挂钩
			phongMat->mDiffuse->bind();

			//高光蒙版贴图,设置采样器使用纹理单元1
			shader->setInt("sampler", 1);
			//将纹理与纹理单元进行挂钩
			phongMat->mSpecularMask->bind();

			//mvp操作如下
			//getModelMatrix()对物体进行,缩放旋转平移
			shader->setMatrix4x4("modelMatrix", mesh->getModelMatrix());
			//相机的视图矩阵(View Matrix),用于将世界坐标转换为相机视角坐标(确定相机位置和朝向)
			shader->setMatrix4x4("viewMatrix", camera->getViewMatrix());
			//投影矩阵将相机坐标系中的3D坐标投影到2D裁剪空间,实现近大远小的透视效果(透视投影)或保持大小不变(正交投影)
			shader->setMatrix4x4("projectionMatrix", camera->getProjectionMatrix());
			//使用Inverse求逆矩阵,然后使用transpose进行转置
			auto normalMatrix = glm::mat3(glm::transpose(glm::inverse(mesh->getModelMatrix())));
			shader->setMatrix3x3("normalMatrix", normalMatrix);
			//光源参数的uniform更新
			shader->setVector3("lightDirection", dirLight->mDirection);         //平行光方向
			shader->setVector3("lightColor", dirLight->mColor);                 //平行光颜色 
			shader->setFloat("specularIntensity", dirLight->mSpecularIntensity);//镜面反射光的强度大小
			shader->setFloat("shiness", phongMat->mShiness);                    //控制物体表面高光区域的次方数
			shader->setVector3("ambientColor", ambLight->mColor);               //环境光颜色
			//相机信息更新
			shader->setVector3("cameraPosition", camera->mPosition);            //相机位置
		}	
			break;
		case MaterialType::WhiteMaterial: {
			//getModelMatrix()对物体进行,缩放旋转平移
			shader->setMatrix4x4("modelMatrix", mesh->getModelMatrix());
			//相机的视图矩阵(View Matrix),用于将世界坐标转换为相机视角坐标(确定相机位置和朝向)
			shader->setMatrix4x4("viewMatrix", camera->getViewMatrix());
			//投影矩阵将相机坐标系中的3D坐标投影到2D裁剪空间,实现近大远小的透视效果(透视投影)或保持大小不变(正交投影)
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

void Renderer::renderPointLight(const std::vector<Mesh*>& meshes, Camera* camera, PointLight* pointLight, AmbientLight* ambLight)
{
	//1.设置当前帧绘制的时候，openGL的必要状态机参数
	//1.1深度检测操作
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	//2.清理画布
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//3.遍历mesh进行绘制
	for (int i = 0; i < meshes.size(); i++) {
		//3.1解包对应内容
		auto mesh = meshes[i];
		auto geometry = mesh->mGeometry;
		auto material = mesh->mMaterial;
		//3.2Mesh的绘制
		//3.2.1决定使用哪个Shader 
		Shader* shader = pickShader(material->mType);
		//3.2.2更新shader的uniform
		shader->begin();

		switch (material->mType) {
		case MaterialType::PhongMaterial: {
			PhongMaterial* phongMat = (PhongMaterial*)material;
			//diffuse贴图,将纹理采样器与纹理单元进行挂钩
			shader->setInt("sampler", 0);
			//将纹理与纹理单元进行挂钩
			phongMat->mDiffuse->bind();

			//将SpecularMask单元进行挂钩,高光蒙版帧更新
			shader->setInt("samplerMaskSampler", 1);
			phongMat->mSpecularMask->bind();

			//相关矩阵
			shader->setMatrix4x4("modelMatrix", mesh->getModelMatrix());//模型矩阵的获得
			shader->setMatrix4x4("viewMatrix", camera->getViewMatrix());//相机视图矩阵(世界坐标系转到相机坐标系)
			shader->setMatrix4x4("projectionMatrix", camera->getProjectionMatrix());//相机投影矩阵(正交投影和透视投影)
			auto normalMatrix = glm::mat3(glm::transpose(glm::inverse(mesh->getModelMatrix())));//法线计算
			shader->setMatrix3x3("normalMatrix", normalMatrix);
			//点光源参数的uniform更新
			shader->setVector3("lightPosition", pointLight->getPosition());
			shader->setVector3("lightColor", pointLight->mColor);
			shader->setFloat("specularIntensity", pointLight->mSpecularIntensity);
			shader->setFloat("k2", pointLight->mK2);
			shader->setFloat("k1", pointLight->mK1);
			shader->setFloat("kc", pointLight->mKc);
			shader->setFloat("shiness", phongMat->mShiness);
			//环境光信息
			shader->setVector3("ambientColor", ambLight->mColor);
			//相机信息更新
			shader->setVector3("cameraPosition", camera->mPosition);

		}
										break;
		case MaterialType::WhiteMaterial: {
			shader->setMatrix4x4("modelMatrix", mesh->getModelMatrix());//模型矩阵的获得
			shader->setMatrix4x4("viewMatrix", camera->getViewMatrix());//相机视图矩阵(世界坐标系转到相机坐标系)
			shader->setMatrix4x4("projectionMatrix", camera->getProjectionMatrix());//相机投影矩阵(正交投影和透视投影)
		}
										break;
		default:
			continue;
		}
		//3.2.3绑定vao
		glBindVertexArray(geometry->getVao());
		//3.2.4执行绘制命令
		glDrawElements(GL_TRIANGLES, geometry->getIndicesCount(), GL_UNSIGNED_INT, 0);
	}
}

void Renderer::renderSpotLight(const std::vector<Mesh*>& meshes, Camera* camera, SpotLight* spotLight, AmbientLight* ambLight)
{
	//1.设置当前帧绘制的时候，openGL的必要状态机参数
	//1.1深度检测操作
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	//2.清理画布
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//3.遍历mesh进行绘制
	for (int i = 0; i < meshes.size(); i++) {
		//3.1解包对应内容
		auto mesh = meshes[i];
		auto geometry = mesh->mGeometry;
		auto material = mesh->mMaterial;
		//3.2Mesh的绘制
		//3.2.1决定使用哪个Shader 
		Shader* shader = pickShader(material->mType);
		//3.2.2更新shader的uniform
		shader->begin();

		switch (material->mType) {
		case MaterialType::PhongMaterial: {
			PhongMaterial* phongMat = (PhongMaterial*)material;
			//diffuse贴图,将纹理采样器与纹理单元进行挂钩
			shader->setInt("sampler", 0);
			//将纹理与纹理单元进行挂钩
			phongMat->mDiffuse->bind();

			//将SpecularMask单元进行挂钩,高光蒙版帧更新
			shader->setInt("samplerMaskSampler", 1);
			phongMat->mSpecularMask->bind();

			//相关矩阵
			shader->setMatrix4x4("modelMatrix", mesh->getModelMatrix());//模型矩阵的获得
			shader->setMatrix4x4("viewMatrix", camera->getViewMatrix());//相机视图矩阵(世界坐标系转到相机坐标系)
			shader->setMatrix4x4("projectionMatrix", camera->getProjectionMatrix());//相机投影矩阵(正交投影和透视投影)
			auto normalMatrix = glm::mat3(glm::transpose(glm::inverse(mesh->getModelMatrix())));//法线计算
			shader->setMatrix3x3("normalMatrix", normalMatrix);
			//探照灯光源参数的uniform更新
			shader->setVector3("lightPosition", spotLight->getPosition());
			shader->setVector3("lightColor", spotLight->mColor);
			shader->setVector3("targetDirection", spotLight->mTargetDirection);
			shader->setFloat("visibleAngle", glm::radians(spotLight->mVisibleAngle));//一定要转为弧度制
			shader->setFloat("specularIntensity", spotLight->mSpecularIntensity);
			shader->setFloat("shiness", phongMat->mShiness);
			//环境光信息
			shader->setVector3("ambientColor", ambLight->mColor);
			//相机信息更新
			shader->setVector3("cameraPosition", camera->mPosition);

		}
										break;
		case MaterialType::WhiteMaterial: {
			shader->setMatrix4x4("modelMatrix", mesh->getModelMatrix());//模型矩阵的获得
			shader->setMatrix4x4("viewMatrix", camera->getViewMatrix());//相机视图矩阵(世界坐标系转到相机坐标系)
			shader->setMatrix4x4("projectionMatrix", camera->getProjectionMatrix());//相机投影矩阵(正交投影和透视投影)
		}
										break;
		default:
			continue;
		}
		//3.2.3绑定vao
		glBindVertexArray(geometry->getVao());
		//3.2.4执行绘制命令
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
