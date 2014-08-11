#version 330

in vec3 vertexNormal;
in vec3 modelSpacePosition;
in vec3 cameraSpacePosition;

uniform vec4 cameraSpaceLightPos;

uniform vec4 lightIntensity;
uniform vec4 ambientIntensity;

uniform vec3 modelSpaceCameraPos;

uniform vec4 matKa;
uniform vec4 matKd;
uniform vec4 matKs;
uniform float matNs;

uniform vec4 carLightPosFL;
uniform vec4 carLightPosFR;
uniform vec4 carLightPosRL;
uniform vec4 carLightPosRR;
uniform vec4 carLightDirF;
uniform vec4 carLightDirR;
uniform vec4 carLightColF;
uniform vec4 carLightColR;
uniform float carLightConeCosF;
uniform float carLightConeCosR;
uniform float carLightAttenF;
uniform float carLightAttenR;

float FogFactor(vec4 glFragCoord, float density);
float SpecularPhongTerm(vec3 lightPos, vec3 surfaceNormal, float exponent, vec3 targetPos);
vec4 Spotlight(vec3 lightPos, vec3 lightDir, float coneCos, vec4 lightColor, float lightAttenuation, vec3 targetPos);
vec4 SpotlightRef(vec3 lightPos, vec3 lightDir, float coneCos, vec4 lightColor, float lightAttenuation, vec3 targetPos);

void main()
{
	vec3 lightDir = normalize(vec3(cameraSpaceLightPos) - cameraSpacePosition);
	
	vec3 surfaceNormal = normalize(vertexNormal);
	
	float cosAngIncidence = dot(surfaceNormal, lightDir);
	cosAngIncidence = clamp(cosAngIncidence, 0, 1);

	float phongTerm = SpecularPhongTerm(vec3(cameraSpaceLightPos),
										surfaceNormal, matNs, cameraSpacePosition);

	float fogFactor = FogFactor(gl_FragCoord, 0.05f);
	vec4 fogColor = vec4(0.4f, 0.4f, 0.5f, 1.0f);
	
	vec4 carSpotlightFL = Spotlight(vec3(carLightPosFL), vec3(carLightDirF), carLightConeCosF,
							carLightColF, carLightAttenF, cameraSpacePosition);
							
	vec4 carSpotlightFR = Spotlight(vec3(carLightPosFR), vec3(carLightDirF), carLightConeCosF,
							carLightColF, carLightAttenF, cameraSpacePosition);
							
	vec4 carSpotlightRL = Spotlight(vec3(carLightPosRL), vec3(carLightDirR), carLightConeCosR,
							carLightColR, carLightAttenR, cameraSpacePosition);
							
	vec4 carSpotlightRR = Spotlight(vec3(carLightPosRR), vec3(carLightDirR), carLightConeCosR,
							carLightColR, carLightAttenR, cameraSpacePosition);
							
	vec4 carSpotlightRefFL = SpotlightRef(vec3(carLightPosFL), vec3(carLightDirF),
							carLightConeCosF, carLightColF, 
							4.0*carLightAttenF, cameraSpacePosition);
							
	vec4 carSpotlightRefFR = SpotlightRef(vec3(carLightPosFR), vec3(carLightDirF),
							carLightConeCosF, carLightColF, 
							4.0*carLightAttenF, cameraSpacePosition);
							
	vec4 carSpotlightRefRL = SpotlightRef(vec3(carLightPosRL), vec3(carLightDirR),
							carLightConeCosR, carLightColR, 
							4.0f*carLightAttenR, cameraSpacePosition);
							
	vec4 carSpotlightRefRR = SpotlightRef(vec3(carLightPosRR), vec3(carLightDirR),
							carLightConeCosR, carLightColR, 
							4.0f*carLightAttenR, cameraSpacePosition);
	
	vec4 outputColor =	(matKa * ambientIntensity)
					+	(matKd * lightIntensity * cosAngIncidence)
					+	(matKs * lightIntensity * phongTerm);
				
			
	outputColor = mix(outputColor, carSpotlightRefFL, carSpotlightRefFL.w);
	outputColor = mix(outputColor, carSpotlightRefFR, carSpotlightRefFR.w);
	outputColor = mix(outputColor, carSpotlightRefRL, carSpotlightRefRL.w);
	outputColor = mix(outputColor, carSpotlightRefRR, carSpotlightRefRR.w);	
	outputColor = mix(outputColor, carSpotlightFL, carSpotlightFL.w);
	outputColor = mix(outputColor, carSpotlightFR, carSpotlightFR.w);
	outputColor = mix(outputColor, carSpotlightRL, carSpotlightRL.w);
	outputColor = mix(outputColor, carSpotlightRR, carSpotlightRR.w);
	outputColor = mix(fogColor, outputColor, fogFactor);
	gl_FragColor = outputColor;
}
