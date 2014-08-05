#version 330

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec4 diffuseColor;

smooth out vec4 interpColor;

uniform vec3 dirToLight;

uniform mat4 modelToCameraMatrix;
uniform mat4 cameraToClipMatrix;

uniform mat3 normalModelToCameraMatrix;

void main()
{
	gl_Position = cameraToClipMatrix * (modelToCameraMatrix * vec4(position, 1.0));
	
//	mat3 normMat = mat3(modelToCameraMatrix);
	
	vec3 normCamSpace = normalize(normalModelToCameraMatrix * normal);
	
//	vec4 dirToLight4 = modelToCameraMatrix * vec4(0.866f, 0.5f, 0.0f, 0.0f);
//	vec3 dirToLight = vec3(1.0, 1.0, 1.0);
//	dirToLight.xyz = dirToLight4.xyz;
	
	float cosAngIncidence = dot(normCamSpace, dirToLight);
	cosAngIncidence = clamp(cosAngIncidence, 0, 1);

	interpColor =  vec4(1.0, 1.0, 1.0, 1.0) * cosAngIncidence;
}
