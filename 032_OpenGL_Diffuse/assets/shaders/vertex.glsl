#version 460 core
layout (location = 0) in vec3 aPos;   //顶点坐标
layout (location = 1) in vec2 aUV;    //纹理坐标
layout (location = 2) in vec3 aNormal;//法线

out vec2 uv;                          //传给 fragment shader 的纹理坐标
out vec3 normal;                      //传给 fragment shader 的法线

uniform mat4 transform;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

//aPos作为attribute（属性）传入shader
//不允许更改的
void main()
{
	vec4 position = vec4(aPos, 1.0);
	position = projectionMatrix * viewMatrix * transform * position;
	gl_Position = position;
	uv = aUV;        //把纹理坐标传给 fragment shader
	normal = aNormal;//把法线坐标传给 fragment shader
}