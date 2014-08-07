#version 330

in vec3 vertexNormal;
in vec3 modelSpacePosition;
in vec3 cameraSpacePosition;

//out vec4 outputColor;

uniform vec3 modelSpaceLightPos;

uniform vec4 lightIntensity;
uniform vec4 ambientIntensity;

uniform vec3 modelSpaceCameraPos;

uniform vec4 matKa;
uniform vec4 matKd;
uniform vec4 matKs;
uniform float matNs;

void main()
{
	vec3 lightDir = normalize(modelSpaceLightPos - modelSpacePosition);
	vec3 surfaceNormal = normalize(vertexNormal);
	
	float cosAngIncidence = dot(surfaceNormal, lightDir);
	cosAngIncidence = clamp(cosAngIncidence, 0, 1);
	
	vec3 viewDirection = normalize(-cameraSpacePosition);
	vec3 reflectDir = reflect(-lightDir, surfaceNormal);
	float phongTerm = dot(viewDirection, reflectDir);
	phongTerm = clamp(phongTerm, 0, 1);
	phongTerm = cosAngIncidence != 0.0 ? phongTerm : 0.0;
	phongTerm = pow(phongTerm, 128.0/matNs);
	
		const float LOG2 = 1.442695;
float z = gl_FragCoord.z / gl_FragCoord.w;
float dens = 0.01f;
vec4 fogCol = 0.5*vec4(1.0f, 1.0f, 1.0f, 2.0f);
float fogFactor = exp2( -dens * 
				   dens * 
				   z * 
				   z * 
				   LOG2 );
fogFactor = clamp(fogFactor, 0.0, 1.0);
	
	vec4 outputColor =	(matKd * ambientIntensity)
				+	(matKd * lightIntensity * cosAngIncidence)
				+	(matKs * lightIntensity * phongTerm);
				
	gl_FragColor = mix(fogCol, outputColor, fogFactor);
}