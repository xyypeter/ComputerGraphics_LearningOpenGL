#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aUV;

out vec3 color;
out vec2 uv;

uniform mat4 transform;


//aPos作为attribute（属性）传入shader原则是上不允许更改的
void main()
{
	vec4 position = vec4(aPos, 1.0);//将aPos(输入的顶点位置)从vec3转换为vec4在齐次坐标中，四维坐标的第四个分量通常设为 1.0，表示位置坐标
	position = transform * position;//使用传入的 transform 矩阵对顶点的位置进行变换
	gl_Position = position;
	color = aColor;
	uv = aUV;
}


