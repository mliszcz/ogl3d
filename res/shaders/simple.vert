#version 330

layout (location = 0) in vec4 position;
layout (location = 1) in vec4 color;

uniform float loopDuration;
uniform float time;

smooth out vec4 theColor;

uniform mat4 perspectiveMatrix;

void main()
{
	float timeScale = 3.14159f * 2.0f / loopDuration;
    
    float currTime = mod(time, loopDuration);
    vec4 totalOffset = vec4(
        cos(currTime * timeScale) * 0.5f,
        sin(currTime * timeScale) * 0.5f,
        0.0f,
        0.0f);
        
    vec4 cameraPos = position + totalOffset;
        
    gl_Position = perspectiveMatrix * cameraPos;
    
	theColor = color;
}
