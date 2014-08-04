#version 330

smooth in vec4 theColor;

out vec4 outputColor;

void main()
{
	outputColor = theColor; //vec4(1.0f, 0.0f, 0.0f, 1.0f);
}
