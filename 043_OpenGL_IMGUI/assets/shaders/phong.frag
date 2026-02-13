#version 460 core
out vec4 FragColor;

in vec2 uv;
in vec3 normal;
in vec3 worldPosition;

uniform sampler2D sampler;	          //diffuse贴图采样器
uniform sampler2D specularMaskSampler;//specularMask贴图采样器
uniform vec3 ambientColor;            //环境光颜色
uniform vec3 cameraPosition;          //相机世界位置
uniform float shiness;                //光斑大小的次方数

//平行光
struct DirectionalLight{
	vec3 direction;          //方向
	vec3 color;              //颜色
	float specularIntensity; //高光反射的强度
};
//点光源
struct PointLight{
	vec3 position;           //位置
	vec3 color;              //颜色
	float k2;                //光源衰减参数
	float k1;                //光源衰减参数
	float kc;                //光源衰减参数
	float specularIntensity; //高光反射的强度
};
//探照灯(聚光灯)
struct SpotLight{
	vec3 position;           //位置
	vec3 targetDirection;    //探照灯朝向的方向
	vec3 color;              //颜色
	float outerLine;         //内层边界线
	float innerLine;         //外层边界线
	float specularIntensity; //高光反射的强度
};

uniform SpotLight spotLight;
uniform DirectionalLight directionalLight;

#define POINT_LIGHT_NUM 4
uniform PointLight pointLights[POINT_LIGHT_NUM];

//计算漫反射光照
//lightColor  :光照的颜色
//objectColor :物体的颜色 
//lightDir    :光照的方向(一定归一化之后传入)  
//normal      :物体本身法线(一定归一化之后传入) 
vec3 calculateDiffuse(vec3 lightColor, vec3 objectColor, vec3 lightDir, vec3 normal){
	float diffuse = clamp(dot(-lightDir, normal), 0.0,1.0);
	vec3 diffuseColor = lightColor * diffuse * objectColor;
	return diffuseColor;
}
//计算镜面反射光照
//lightColor:光的颜色
//lightDir  :光的方向
//normal    :物体法线
//viewDir   :看向物体的方向 
//intensity :镜面反射的强度
vec3 calculateSpecular(vec3 lightColor, vec3 lightDir, vec3 normal, vec3 viewDir, float intensity){
	//1 防止背面光效果
	float dotResult = dot(-lightDir, normal);
	float flag = step(0.0, dotResult);
	vec3 lightReflect = normalize(reflect(lightDir,normal));
	//2 jisuan specular
	float specular = max(dot(lightReflect,-viewDir), 0.0);
	//3 控制光斑大小
	specular = pow(specular, shiness);
	//float specularMask=texture(specularMaskSampler,uv).r;不一定每一个都需要
	//4 计算最终颜色
	vec3 specularColor = lightColor * specular * flag * intensity;
	return specularColor;
}

//计算探照灯光照
vec3 calculateSpotLight(SpotLight light, vec3 normal, vec3 viewDir){
	//计算光照的通用数据
	vec3 objectColor  = texture(sampler, uv).xyz;
	vec3 lightDir = normalize(worldPosition - light.position);
	vec3 targetDir = normalize(light.targetDirection);
	//计算spotlight的照射范围
	float cGamma = dot(lightDir, targetDir);
	float intensity =clamp( (cGamma - light.outerLine) / (light.innerLine - light.outerLine), 0.0, 1.0);
	//1 计算diffuse
	vec3 diffuseColor = calculateDiffuse(light.color,objectColor, lightDir,normal);
	//2 计算specular
	vec3 specularColor = calculateSpecular(light.color, lightDir,normal, viewDir,light.specularIntensity); 
	return (diffuseColor + specularColor)*intensity;
}
//计算平行光光照
vec3 calculateDirectionalLight(DirectionalLight light, vec3 normal ,vec3 viewDir){
	//计算光照的通用数据
	vec3 objectColor  = texture(sampler, uv).xyz;
	vec3 lightDir = normalize(light.direction);
	//1 计算diffuse
	vec3 diffuseColor = calculateDiffuse(light.color,objectColor, lightDir,normal);
	//2 计算specular
	vec3 specularColor = calculateSpecular(light.color, lightDir,normal, viewDir,light.specularIntensity); 
	return diffuseColor + specularColor;
}
vec3 calculatePointLight(PointLight light, vec3 normal ,vec3 viewDir){
	//计算光照的通用数据
	vec3 objectColor  = texture(sampler, uv).xyz;
	vec3 lightDir = normalize(worldPosition - light.position);
	//计算衰减
	float dist = length(worldPosition - light.position);
	float attenuation = 1.0 / (light.k2 * dist * dist + light.k1 * dist + light.kc);
	//1 计算diffuse
	vec3 diffuseColor = calculateDiffuse(light.color,objectColor, lightDir,normal);
	//2 计算specular
	vec3 specularColor = calculateSpecular(light.color, lightDir,normal, viewDir,light.specularIntensity); 
	return (diffuseColor + specularColor)*attenuation;
}

void main()
{
	vec3 result = vec3(0.0,0.0,0.0);
	//计算光照的通用数据
	vec3 objectColor  = texture(sampler, uv).xyz;
	vec3 normalN = normalize(normal);
	vec3 lightDirN = normalize(worldPosition - spotLight.position);
	vec3 viewDir = normalize(worldPosition - cameraPosition);
	vec3 targetDirN = normalize(spotLight.targetDirection);

	result += calculateSpotLight(spotLight, normalN, viewDir);
	result += calculateDirectionalLight(directionalLight,normalN, viewDir);
	for(int i = 0;i < POINT_LIGHT_NUM ;i++){
	    result += calculatePointLight(pointLights[i],normalN, viewDir);
	}
	
	//环境光计算
	vec3 ambientColor = objectColor * ambientColor;
    //输出最终的颜色
	vec3 finalColor = result + ambientColor;
	FragColor = vec4(finalColor, 1.0);
}