#version 330

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
//layout(location = 2) in vec4 diffuseColor;

//smooth out vec4 interpColor;

out vec4 diffuseColor;
out vec3 vertexNormal;
out vec3 modelSpacePosition;

//uniform vec3 dirToLight;
//uniform vec4 lightIntensity;
//uniform vec4 ambientIntensity;

uniform mat4 modelToCameraMatrix;
//uniform mat3 normalModelToCameraMatrix;

//uniform vec4 matKa;
//uniform vec4 matKd;
//uniform vec4 matKs;
//uniform vec4 matTr;
//uniform vec4 matKe;
//uniform float matNs;


uniform mat4 cameraToClipMatrix;

void main()
{

	gl_Position = cameraToClipMatrix * (modelToCameraMatrix * vec4(position, 1.0));

//	vec3 normCamSpace = normalize(normalModelToCameraMatrix * normal);
	
//	float cosAngIncidence = dot(normCamSpace, dirToLight);
//	cosAngIncidence = clamp(cosAngIncidence, 0, 1);
	
	modelSpacePosition = position;
	vertexNormal = normal;
	
//	interpColor = lightIntensity * matKd * cosAngIncidence
//				+ matKa * ambientIntensity;
}
