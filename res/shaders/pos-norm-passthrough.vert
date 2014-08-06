#version 330

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;

out vec3 vertexNormal;
out vec3 modelSpacePosition;
out vec3 cameraSpacePosition;

uniform mat4 modelToCameraMatrix;
uniform mat4 cameraToClipMatrix;

void main()
{
	vertexNormal = normal;
	
	modelSpacePosition = position;
	vec4 tmpCameraSpacePosition = modelToCameraMatrix * vec4(modelSpacePosition, 1.0);
	cameraSpacePosition = vec3(tmpCameraSpacePosition);
	gl_Position = cameraToClipMatrix * tmpCameraSpacePosition;
}
