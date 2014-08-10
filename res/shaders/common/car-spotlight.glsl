#version 330

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