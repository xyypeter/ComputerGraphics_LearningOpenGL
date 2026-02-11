#include "geometry.h"
#include <vector>

Geometry::Geometry() {

}

Geometry::~Geometry() {
	if (mVao != 0) {
		glDeleteVertexArrays(1, &mVao);
	}
	if (mPosVbo != 0) {
		glDeleteBuffers(1, &mPosVbo);
	}
	if (mUvVbo != 0) {
		glDeleteBuffers(1, &mUvVbo);
	}
	if (mEbo != 0) {
		glDeleteBuffers(1, &mEbo);
	}
	if (mNormalVbo != 0) {
		glDeleteBuffers(1, &mNormalVbo);
	}
}

Geometry* Geometry::createBox(float size) {
	Geometry* geometry = new Geometry();
	geometry->mIndicesCount = 36;
	float halfSize = size / 2.0f;
	float positions[] = {//顶点位置 positions[]:每个面的顶点坐标，共6个面，每面4个顶点，每个顶点3个分量(x, y, z)
		// Front face
		-halfSize, -halfSize, halfSize, halfSize, -halfSize, halfSize, halfSize, halfSize, halfSize, -halfSize, halfSize, halfSize,
		// Back face
		-halfSize, -halfSize, -halfSize, -halfSize, halfSize, -halfSize, halfSize, halfSize, -halfSize, halfSize, -halfSize, -halfSize,
		// Top face
		-halfSize, halfSize, halfSize, halfSize, halfSize, halfSize, halfSize, halfSize, -halfSize, -halfSize, halfSize, -halfSize,
		// Bottom face
		-halfSize, -halfSize, -halfSize, halfSize, -halfSize, -halfSize, halfSize, -halfSize, halfSize, -halfSize, -halfSize, halfSize,
		// Right face
		halfSize, -halfSize, halfSize, halfSize, -halfSize, -halfSize, halfSize, halfSize, -halfSize, halfSize, halfSize, halfSize,
		// Left face
		-halfSize, -halfSize, -halfSize, -halfSize, -halfSize, halfSize, -halfSize, halfSize, halfSize, -halfSize, halfSize, -halfSize
	};
	float uvs[] = {//纹理坐标 uvs[],用于表达百分比位置的横纵百分比，是每个顶点的贴图坐标 (u, v)，用来映射 2D 纹理图像到 3D 模型表面
		0.0, 0.0, 1.0, 0.0, 1.0, 1.0, 0.0, 1.0,
		0.0, 0.0, 1.0, 0.0, 1.0, 1.0, 0.0, 1.0,
		0.0, 0.0, 1.0, 0.0, 1.0, 1.0, 0.0, 1.0,
		0.0, 0.0, 1.0, 0.0, 1.0, 1.0, 0.0, 1.0,
		0.0, 0.0, 1.0, 0.0, 1.0, 1.0, 0.0, 1.0,
		0.0, 0.0, 1.0, 0.0, 1.0, 1.0, 0.0, 1.0,
	};
	float normals[] = {//注意:重合顶点的法线是不一样的，所以需要单独编写
		//前面
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		//后面
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f,
		//上面
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		//下面
		0.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		//右面
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		//左面
		-1.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,
	};
	unsigned int indices[] = {
		0, 1, 2, 2, 3, 0,       // Front face
		4, 5, 6, 6, 7, 4,       // Back face
		8, 9, 10, 10, 11, 8,    // Top face
		12, 13, 14, 14, 15, 12, // Bottom face
		16, 17, 18, 18, 19,  16,// Right face
		20, 21, 22, 22, 23, 20  // Left face
	};
	//2 VBO创建
	GLuint& posVbo = geometry->mPosVbo, uvVbo = geometry->mUvVbo, normalVbo = geometry->mNormalVbo ;
	glGenBuffers(1, &posVbo);
	glBindBuffer(GL_ARRAY_BUFFER, posVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);
	glGenBuffers(1, &uvVbo);
	glBindBuffer(GL_ARRAY_BUFFER, uvVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(uvs), uvs, GL_STATIC_DRAW);
	glGenBuffers(1, &normalVbo);
	glBindBuffer(GL_ARRAY_BUFFER, normalVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(normals), normals, GL_STATIC_DRAW);
	//3 EBO创建
	glGenBuffers(1, &geometry->mEbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry->mEbo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	//4 VAO创建
	glGenVertexArrays(1, &geometry->mVao);
	glBindVertexArray(geometry->mVao);
	glBindBuffer(GL_ARRAY_BUFFER, posVbo);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
	glBindBuffer(GL_ARRAY_BUFFER, uvVbo);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*)0);
	glBindBuffer(GL_ARRAY_BUFFER, normalVbo);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
	//5加入ebo到当前的vao
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry->mEbo);
	glBindVertexArray(0);
	return geometry;
}

Geometry* Geometry::createSphere(float radius) {
	Geometry* geometry = new Geometry();
	//目标：1 位置 2 uv 3 索引
	//1 主要变量声明
	std::vector<GLfloat> positions{};
	std::vector<GLfloat> uvs{};
	std::vector<GLfloat> normals{};
	std::vector<GLuint> indices{};
	//声明纬线与经线的数量
	int numLatLines = 60;//纬线
	int numLongLines = 60;//经线
	//2 通过两层循环（纬线在外，经线在内）->位置、uv、法线
	for (int i = 0; i <= numLatLines; i++) {
		for (int j = 0; j <= numLongLines; j++) {
			float phi = i * glm::pi<float>() / numLatLines;
			float theta = j * 2 * glm::pi<float>() / numLongLines;
			float y = radius * cos(phi);
			float x = radius * sin(phi) * cos(theta);
			float z = radius * sin(phi) * sin(theta);
			positions.push_back(x);
			positions.push_back(y);
			positions.push_back(z);
			float u = 1.0 - (float)j / (float)numLongLines;
			float v = 1.0 - (float)i / (float)numLatLines;
			uvs.push_back(u);
			uvs.push_back(v);
			//注意：法线方向没有问题，法线的长度不为1
			normals.push_back(x);//使用顶点位置减去原点位置(0,0,0),即可得到法线，所以直接添加即可
			normals.push_back(y);
			normals.push_back(z);
		}
	}
	//3 通过两层循环（这里没有=号）->顶点索引
	for (int i = 0; i < numLatLines; i++) {
		for (int j = 0; j < numLongLines; j++) {
			int p1 = i * (numLongLines + 1) + j;
			int p2 = p1 + numLongLines + 1;
			int p3 = p1 + 1;
			int p4 = p2 + 1;
			indices.push_back(p1);
			indices.push_back(p2);
			indices.push_back(p3);
			indices.push_back(p3);
			indices.push_back(p2);
			indices.push_back(p4);
		}
	}
	//4 生成vbo与vao
	GLuint& posVbo = geometry->mPosVbo, uvVbo = geometry->mUvVbo, normalVbo = geometry->mNormalVbo;
	glGenBuffers(1, &posVbo);
	glBindBuffer(GL_ARRAY_BUFFER, posVbo);
	glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(float), positions.data(), GL_STATIC_DRAW);
	glGenBuffers(1, &uvVbo);
	glBindBuffer(GL_ARRAY_BUFFER, uvVbo);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(float), uvs.data(), GL_STATIC_DRAW);
	glGenBuffers(1, &normalVbo);
	glBindBuffer(GL_ARRAY_BUFFER, normalVbo);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(float), normals.data(), GL_STATIC_DRAW);
	glGenBuffers(1, &geometry->mEbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry->mEbo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);
	glGenVertexArrays(1, &geometry->mVao);
	glBindVertexArray(geometry->mVao);
	glBindBuffer(GL_ARRAY_BUFFER, posVbo);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
	glBindBuffer(GL_ARRAY_BUFFER, uvVbo);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*)0);
	glBindBuffer(GL_ARRAY_BUFFER, normalVbo);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry->mEbo);
	glBindVertexArray(0);
	geometry->mIndicesCount = indices.size();
	return geometry;
}


Geometry* Geometry::createPlane(float width, float height) {
	Geometry* geometry = new Geometry();
	geometry->mIndicesCount = 6; // 平面由2个三角形组成（6个顶点索引）
	float halfW = width / 2.0f;  // 半宽
	float halfH = height / 2.0f; // 半高
	// 四个顶点坐标(以原点为中心)
	float positions[] = {
		-halfW, -halfH, 0.0f,    // 左下
		halfW, -halfH, 0.0f,     // 右下
		halfW, halfH, 0.0f,      // 右上
		-halfW, halfH, 0.0f,     // 左上
	};
	//定义纹理坐标(UV)
	float uvs[] = {
		0.0f, 0.0f,// 左下
		1.0f, 0.0f,// 右下
		1.0f, 1.0f,// 右上
		0.0f, 1.0f // 左上
	};
	//定义法线向量
	float normals[] = {
		0.0f, 0.0f, 1.0f,// 所有顶点法线均指向Z轴正方向
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
	};
	//定义顶点索引（三角形连接顺序）
	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	//2 VBO创建(创建顶点缓冲区VBO)
	GLuint& posVbo = geometry->mPosVbo, uvVbo = geometry->mUvVbo, normalVbo = geometry->mNormalVbo;
	//2.1 位置缓冲区
	glGenBuffers(1, &posVbo);
	glBindBuffer(GL_ARRAY_BUFFER, posVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);
	//2.2 UV坐标缓冲区
	glGenBuffers(1, &uvVbo);
	glBindBuffer(GL_ARRAY_BUFFER, uvVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(uvs), uvs, GL_STATIC_DRAW);
	//2.3 法线缓冲区
	glGenBuffers(1, &normalVbo);
	glBindBuffer(GL_ARRAY_BUFFER, normalVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(normals), normals, GL_STATIC_DRAW);

	//3 EBO创建(创建索引缓冲区)
	glGenBuffers(1, &geometry->mEbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry->mEbo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//4 创建顶点数组对象(VAO)并配置属性
	glGenVertexArrays(1, &geometry->mVao);
	glBindVertexArray(geometry->mVao);
	//4.1 位置属性（属性索引0）
	glBindBuffer(GL_ARRAY_BUFFER, posVbo);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
	//4.2 UV属性（属性索引1）
	glBindBuffer(GL_ARRAY_BUFFER, uvVbo);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*)0);
	//4.3 法线属性（属性索引2）
	glBindBuffer(GL_ARRAY_BUFFER, normalVbo);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
	//5 加入ebo到当前的vao
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry->mEbo);
	//6 解绑VAO并返回对象
	glBindVertexArray(0);
	return geometry;
}
