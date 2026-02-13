#version 460 core
out vec4 FragColor;

in vec2 uv;
in vec3 normal;
in vec3 worldPosition;
uniform sampler2D sampler;          //diffuser贴图采样器
uniform sampler2D samplerMaskSapler;//高光蒙版采样器

//光源参数
uniform vec3  ambientColor;
uniform vec3  lightPosition;
uniform vec3  lightColor;
//uniform float k2;
//uniform float k1;
//uniform float kc;
uniform vec3  targetDirection;
//uniform float  visibleAngle;
//虽然可以像上述 visibleAngle直接传入角度,然后在下方的shader代码中计算,但是三角函数在显卡中的计算依然压力比较大,
//所以建议在CPU中计算完成后，传入shader
uniform float innerLine;//内边界
uniform float outerLine;//外边界

//相机世界位置
uniform vec3  cameraPosition;
uniform float specularIntensity;
uniform float shiness;

void main(){
	//计算光照的通用数据
	vec3 objectColor  = texture(sampler, uv).xyz;
	vec3 normalN = normalize(normal);
	vec3 lightDirN = normalize(worldPosition-lightPosition);
	vec3 viewDir = normalize(worldPosition - cameraPosition);
	vec3 targetDirN = normalize(targetDirection);//补充探照灯的朝向
	//计算spotlight的照射范围
	float cGamma = dot(lightDirN,targetDirN);
	float spotIntensity=clamp((cGamma-outerLine)/(innerLine-outerLine),0.0,1.0);
	//float cTheta   = dot(lightDirN,targetDirN);
	//float cVisible = cos(visibleAngle);        //此处注意一定是弧度制
	//float spotFlag = step(cVisible,cTheta);

	//补充衰减值的计算
	//float dist=length(worldPosition-lightPosition);
	//float attenuation=1.0/(k2*dist*dist+k1*dist+kc);

	//计算diffuse
	float diffuse = clamp(dot(-lightDirN, normalN), 0.0,1.0);
	vec3 diffuseColor = lightColor * diffuse * objectColor;
	//计算specular防止背面光效果
	float dotResult = dot(-lightDirN, normalN);
	float flag = step(0.0, dotResult);
	vec3 lightReflect = normalize(reflect(lightDirN,normalN));
	float specular = max(dot(lightReflect,-viewDir), 0.0);
	//控制光斑大小
	specular = pow(specular, shiness);
	float samplerMask=texture(samplerMaskSapler,uv).r;
	vec3 specularColor = lightColor * specular * flag * specularIntensity*samplerMask;
	//环境光计算
	vec3 ambientColor = objectColor * ambientColor;
	//最终颜色输出
	//vec3 finalColor = (diffuseColor + specularColor)*attenuation + ambientColor;
	//vec3 finalColor = (diffuseColor + specularColor)*spotFlag + ambientColor;
	vec3 finalColor = (diffuseColor + specularColor)*spotIntensity + ambientColor;
	FragColor = vec4(finalColor, 1.0);
}
