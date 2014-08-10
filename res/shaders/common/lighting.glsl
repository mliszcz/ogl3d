#version 330

// all calculations in camera space

vec4 Spotlight(vec3 lightPos, vec3 lightDir, float coneCos, vec4 lightColor, vec3 targetPos)
{
	vec3 targetDir = targetPos-lightPos;
	float targetDist = length(targetDir);
	
	float targetCos = dot(normalize(lightDir), normalize(targetDir));
	
	float fDif = 1.0-coneCos;
	float fFactor = clamp((targetCos-coneCos)/fDif, 0.0, 1.0);
	
	if(targetCos > coneCos)
		return lightColor * fFactor/targetDist;
		
	return vec4(0.0, 0.0, 0.0, 0.0);
}

float SpecularPhongTerm(vec3 lightPos, vec3 surfaceNormal, float exponent, vec3 targetPos)
{
	vec3 lightDir = normalize(lightPos-targetPos);

	float cosAngIncidence = dot(surfaceNormal, lightDir);
	cosAngIncidence = clamp(cosAngIncidence, 0, 1);
	
	vec3 viewDirection = normalize(-targetPos);
	vec3 reflectDir = reflect(-lightDir, surfaceNormal);
	float phongTerm = dot(viewDirection, reflectDir);
	phongTerm = clamp(phongTerm, 0, 1);
	phongTerm = cosAngIncidence != 0.0 ? phongTerm : 0.0;
	phongTerm = pow(phongTerm, exponent);
	
	return phongTerm;
}

float FogFactor(vec4 glFragCoord, float density)
{
	const float LOG2 = 1.442695;
	float z = glFragCoord.z / glFragCoord.w;
	float fogFactor = exp2(-density*density*z*z*LOG2);
	return clamp(fogFactor, 0.0, 1.0);
}
