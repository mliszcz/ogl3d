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

vec4 GetSpotLightColorAt(vec3 lightPosCam, vec3 lightDirCam, float fConeCosine, vec3 vWorldPos)
{
  float fDistance = distance(vWorldPos, lightPosCam);

  vec3 vDir = vWorldPos-lightPosCam;
  vDir = normalize(vDir);
  
  float fCosine = dot(lightDirCam, vDir);
  float fDif = 1.0-fConeCosine;
  float fFactor = clamp((fCosine-fConeCosine)/fDif, 0.0, 1.0);

  vec3 vColor = vec3(1.0f, 1.0f, 1.0f);
  if(fCosine > fConeCosine)
    return vec4(vColor, 1.0)*fFactor/(fDistance*1);

  return vec4(0.0, 0.0, 0.0, 0.0);
}

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
	phongTerm = pow(phongTerm, 1.0f);
	
	vec4 baseColor = texture(textureSampler, textureCoord);
	vec4 white = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	
	const float LOG2 = 1.442695;
	float z = gl_FragCoord.z / gl_FragCoord.w;
	float dens = 0.05f;
	vec4 fogCol = vec4(0.4f, 0.4f, 0.5f, 1.0f);
	float fogFactor = exp2(-dens*dens*z*z*LOG2);
	fogFactor = clamp(fogFactor, 0.0, 1.0);
	
	vec4 outputColor =	(baseColor * ambientIntensity)
				+	(baseColor * lightIntensity * cosAngIncidence)
				+	(baseColor * lightIntensity * phongTerm);
				
	vec4 spotlight = GetSpotLightColorAt(vec3(gLightPosCam), vec3(gLightDirCam), gConeCosine, cameraSpacePosition);
				
	//gl_FragColor = mix(fogCol, outputColor, fogFactor);
	gl_FragColor = mix(spotlight, mix(fogCol, outputColor, fogFactor), 0.5f);
				
}