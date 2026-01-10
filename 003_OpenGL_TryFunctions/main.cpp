#include <iostream>
#include <glad/glad.h>//坑：头文件引用，glad需要放在GLFW上
#include <GLFW/glfw3.h>
using namespace std;

//声明且实现一个响应窗体大小变化的函数
void frameBufferSizeCallBack(GLFWwindow* window, int width, int height) {
	cout << "窗体最新大小：" << width << " , " << height << endl;
	// 窗体发生变化是，随时更新视口的大小
	glViewport(0, 0, width, height);
}

//声明且实现一个键盘消息回调函数
//key:     字母按键码
//scancode:物理按键码(按下的键位在键盘的那个位置)
//action:  按下还是抬起
//mods:    是否有shift和ctrl 
void keyCallBack(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_W) {
		//触发了w
	}
	if (action == GLFW_PRESS) {
		//键位按下
	}
	if (action == GLFW_RELEASE) {
		//键位抬起
	}
	if (mods == GLFW_MOD_CONTROL) {
		//按下ctrl的同时，按下了key
	}
	if (mods == GLFW_MOD_SHIFT) {
		//按下shift的同时，按下了key
	}
	cout << "按下了：" << key << endl;
	cout << "action：" << action << endl;
	cout << "mods：" << mods << endl;
}

int main() {
//1 初始化GLFW基本环境
	glfwInit();
	//1.1 设置OpenGL主版本号、次版本号
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	//1.2 设置OpenGL启用核心模式（非立即渲染模式）
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//2 创建窗体对象
	GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGLStudy", NULL, NULL);
	//2.1设置当前窗体对象为OpenGL的绘制舞台
	glfwMakeContextCurrent(window);
	//2.2设置监听，监听窗体大小变化的消息
	glfwSetFramebufferSizeCallback(window, frameBufferSizeCallBack);
	//2.3设置监听，监听键盘消息
	glfwSetKeyCallback(window, keyCallBack);
	//2.4使用glad加载所有当前版本opengl的函数
	//原因是OpenGL是一个规范标准，而不是具体的实现，实现靠各家显卡厂商编写的驱动程序，程序如果需要运行，就需要向显卡驱动查询函数的具体位置并且加载进来 
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		cout << "Failed to initialize GLAD" << endl;
		return -1;
	}
	//设置opengl视口(设置窗口中openGL负责渲染的区域，即为视口)
	// x,y：表示相对窗口左下角的起始位置
	// width,height：表示渲染区域的长度、宽度 
	// 注意状态设置，执行一次即可
	glViewport(0, 0, 800, 600);
	//设置画布清理颜色(状态设置，执行一次即可)
	//参数1:red
	//参数2:green
	//参数3:blue
	//参数4:alpha
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

//3 执行窗体循环
	while (!glfwWindowShouldClose(window)) {
		//接收并分发窗体消息
		//检查消息队列是否有需要处理的鼠标、键盘等消息
		//如果有的话就将消息批量处理，清空队列
		glfwPollEvents();
		//执行opengl画布清理操作
		glClear(GL_COLOR_BUFFER_BIT);//执行OpenGL的操作，意味着每帧需要执行

		//渲染操作
		//TODO......

		//切换双缓存
		glfwSwapBuffers(window);//执行OpenGL的操作，意味着每帧需要执行
	}
	//4 退出程序前做相关清理
	glfwTerminate();
	return 0;
}