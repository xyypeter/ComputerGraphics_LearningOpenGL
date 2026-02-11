#version 460 core
out vec4 FragColor;//最终输出颜色

in vec2 uv;
in vec3 normal;

uniform sampler2D sampler;  //传入的纹理
uniform vec3 lightDirection;//光照方向
uniform vec3 lightColor;    //光照强度(颜色)

void main(){
	//1 获取物体的当前像素的颜色(从纹理中采样像素颜色)
	vec3 objectColor  = texture(sampler, uv).xyz;
	//2 准备diffuse（漫反射）相关的各类数据
	//2.1法线归一化
	vec3 normalN = normalize(normal);       
	//2.2光照方向归一化
	vec3 lightDirN = normalize(lightDirection);
	//3 diffuse对光的吸收率-求Cos（clamp的作用是,若小于最小边界，剪裁至最小边界，若大于最大边界，剪裁至最大边界，处于区间之中，原值输出）
	float diffuse = clamp(dot(-lightDirN, normalN), 0.0,1.0);
	//4 计算最终颜色(人眼看到的颜色)   lightColor * diffuse表示物体吸收的颜色 
	vec3 finalColor = lightColor * diffuse * objectColor;
	//5 输出最终颜色，finalColor是一个三维向量（vec3），包含RGB三个分量，即红色、绿色和蓝色通道的值，设置透明度1.0，组合一下已四维向量输出
	FragColor = vec4(finalColor, 1.0);
}

