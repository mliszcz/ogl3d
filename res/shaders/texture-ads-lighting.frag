#version 330

in vec3 vertexNormal;
in vec2 textureCoord;
in vec3 modelSpacePosition;
in vec3 cameraSpacePosition;

//out vec4 outputColor;

//uniform vec3 modelSpaceLightPos;
uniform vec4 cameraSpaceLightPos;

//uniform mat4 cameraToModelMatrix;

uniform vec4 lightIntensity;
uniform vec4 ambientIntensity;

uniform vec3 modelSpaceCameraPos;

uniform sampler2D textureSampler;

uniform vec4 gLightPosCam;
uniform vec4 gLightDirCam;
uniform float gConeCosine;

float FogFactor(vec4 glFragCoord, float density);
float SpecularPhongTerm(vec3 lightPos, vec3 surfaceNormal, float exponent, vec3 targetPos);
vec4 Spotlight(vec3 lightPos, vec3 lightDir, float coneCos, vec4 lightColor, vec3 targetPos);

void main()
{
	//vec3 lightDir = normalize(vec3(cameraToModelMatrix * cameraSpaceLightPos) - modelSpacePosition);
	vec3 lightDir = normalize(vec3(cameraSpaceLightPos) - cameraSpacePosition);
	
	vec3 surfaceNormal = normalize(vertexNormal);
	
	float cosAngIncidence = dot(surfaceNormal, lightDir);
	cosAngIncidence = clamp(cosAngIncidence, 0, 1);
	
	float phongTerm = SpecularPhongTerm(vec3(cameraSpaceLightPos),
										surfaceNormal, 100.0f, cameraSpacePosition);
	
	vec4 baseColor = texture(textureSampler, textureCoord);
	vec4 white = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	
	float fogFactor = FogFactor(gl_FragCoord, 0.05f);
	vec4 fogCol = vec4(0.4f, 0.4f, 0.5f, 1.0f);
	
	vec4 outputColor =	(baseColor * ambientIntensity)
				+	(baseColor * lightIntensity * cosAngIncidence)
				+	(baseColor * lightIntensity * phongTerm);
				
	vec4 spotlight = Spotlight(vec3(gLightPosCam), vec3(gLightDirCam), gConeCosine, white, cameraSpacePosition);
	
	//fogCol = mix(spotlight, fogCol, 0.5f);
				
	//gl_FragColor = mix(fogCol, outputColor, fogFactor);
	gl_FragColor = mix(spotlight, mix(fogCol, outputColor, fogFactor), 0.5f);
	//gl_FragColor = mix(mix(spotlight, fogCol, 0.5f), outputColor, fogFactor);
				
}