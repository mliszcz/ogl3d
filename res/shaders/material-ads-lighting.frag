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

vec4 GetSpotLightColorAt(vec3 lightPosCam, vec3 lightDirCam, float fConeCosine, vec3 vWorldPos);

void main()
{
	//vec3 lightDir = normalize(vec3(cameraToModelMatrix * cameraSpaceLightPos) - modelSpacePosition);
	vec3 lightDir = normalize(vec3(cameraSpaceLightPos) - cameraSpacePosition);
	
	vec3 surfaceNormal = normalize(vertexNormal);
	
	float cosAngIncidence = dot(surfaceNormal, lightDir);
	cosAngIncidence = clamp(cosAngIncidence, 0, 1);
	
	vec3 viewDirection = normalize(-cameraSpacePosition);
	vec3 reflectDir = reflect(-lightDir, surfaceNormal);
	float phongTerm = dot(viewDirection, reflectDir);
	phongTerm = clamp(phongTerm, 0, 1);
	phongTerm = cosAngIncidence != 0.0 ? phongTerm : 0.0;
	phongTerm = pow(phongTerm, matNs);
	
	const float LOG2 = 1.442695;
	float z = gl_FragCoord.z / gl_FragCoord.w;
	float dens = 0.05f;
	vec4 fogCol = vec4(0.4f, 0.4f, 0.5f, 1.0f);
	float fogFactor = exp2(-dens*dens*z*z*LOG2);
	fogFactor = clamp(fogFactor, 0.0, 1.0);
	
	vec4 spotlight = GetSpotLightColorAt(vec3(gLightPosCam), vec3(gLightDirCam), gConeCosine, cameraSpacePosition);
	
	vec4 outputColor =	(matKa * ambientIntensity)
				+	(matKd * lightIntensity * cosAngIncidence)
				+	(matKs * lightIntensity * phongTerm);
				
	gl_FragColor = mix(spotlight, mix(fogCol, outputColor, fogFactor), 0.5f);
}