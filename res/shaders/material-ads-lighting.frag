#version 330

in vec3 vertexNormal;
in vec3 modelSpacePosition;
in vec3 cameraSpacePosition;

uniform vec4 cameraSpaceLightPos;

uniform vec4 lightIntensity;
uniform vec4 ambientIntensity;

//uniform mat4 cameraToModelMatrix;

uniform vec3 modelSpaceCameraPos;

uniform vec4 matKa;
uniform vec4 matKd;
uniform vec4 matKs;
uniform float matNs;

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
										surfaceNormal, matNs, cameraSpacePosition);

	float fogFactor = FogFactor(gl_FragCoord, 0.05f);
	vec4 fogCol = vec4(0.4f, 0.4f, 0.5f, 1.0f);
	
	vec4 spotlight = Spotlight(vec3(gLightPosCam), vec3(gLightDirCam),
								gConeCosine, vec4(1.0f, 1.0f, 1.0f, 1.0f), cameraSpacePosition);
	
	vec4 outputColor =	(matKa * ambientIntensity)
				+	(matKd * lightIntensity * cosAngIncidence)
				+	(matKs * lightIntensity * phongTerm);
				
				
	//fogCol = mix(spotlight, fogCol, 0.5f);
				
	//gl_FragColor = mix(fogCol, outputColor, fogFactor);
	
	gl_FragColor = mix(spotlight, mix(fogCol, outputColor, fogFactor), 0.5f);
	//gl_FragColor = mix(mix(spotlight, fogCol, 0.5f), outputColor, fogFactor);
}