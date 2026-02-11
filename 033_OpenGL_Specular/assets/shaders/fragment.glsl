#version 460 core
out vec4 FragColor;
in vec2 uv;
in vec3 normal;
in vec3 worldPosition;          //当前像素的世界位置
uniform sampler2D sampler;
//光源参数
uniform vec3 lightDirection;    //光照方向
uniform vec3 lightColor;        //光照颜色
uniform vec3 ambientColor;      //环境光颜色
uniform vec3 cameraPosition;    //相机世界位置
uniform float specularIntensity;//镜面反射强度

void main()
{
	//计算光照的通用数据
	vec3 objectColor  = texture(sampler, uv).xyz;            //物体颜色
	vec3 normalN = normalize(normal);                        //法线归一化
	vec3 lightDirN = normalize(lightDirection);              //入射光归一化
	vec3 viewDir = normalize(worldPosition - cameraPosition);//视线方向的计算,且归一化
	//计算diffuse(慢反射)
	float diffuse = clamp(dot(-lightDirN, normalN), 0.0,1.0);
	vec3 diffuseColor = lightColor * diffuse * objectColor;
	//防止背面光效果
	float dotResult = dot(-lightDirN, normalN);               //通过入射光和法线的点乘,得到照射情况(dotResult>0正面OR背面<0)
	float flag = step(0.0, dotResult);                        //通过step,dotResult>0返回原值，<0返回0 
	vec3 lightReflect = normalize(reflect(lightDirN,normalN));//计算镜面反射光线方向
	//计算specular(高光反射)
	float specular = max(dot(lightReflect,-viewDir), 0.0);
	//控制光斑大小
	specular = pow(specular, 64);
	//计算高光反射颜色
	vec3 specularColor = lightColor * specular * flag * specularIntensity;
	//环境光计算
	vec3 ambientColor = objectColor * ambientColor;
	//计算最终颜色
	vec3 finalColor = diffuseColor + specularColor + ambientColor;
	//输出最终的颜色
	FragColor = vec4(finalColor, 1.0);
}
