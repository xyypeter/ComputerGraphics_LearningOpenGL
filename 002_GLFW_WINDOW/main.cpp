#include <iostream>
#include <glad/glad.h>//此处需要注意坑：头文件引用，glad需要放在GLFW上
#include <GLFW/glfw3.h>

int main() 
{
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

//3 执行窗体循环
	while (!glfwWindowShouldClose(window)) {
		//此处用于接收并分发窗体消息，接收消息队列是否有需要处理的鼠标、键盘消息
		//如果有的话就讲消息批量处理，清空队列
		glfwPollEvents();
	}

//4 退出程序前做相关清理
	glfwTerminate();
	return 0;
}