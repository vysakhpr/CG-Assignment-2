#version 330
in vec3 Color0;

out vec4 FragColor;
struct DirectionalLight
{
	vec3 Color;
	float AmbientIntensity;
	vec3 Direction;
	float DiffuseIntensity;
};

struct PositionalLight
{	
	vec3 Color;
	vec3 Position;
	float AmbientIntensity;
	float DiffuseIntensity;
	struct 
	{
		float Constant;
		float Linear;
		float Exponential;
	}Attenuation;
};

uniform DirectionalLight gDirectionalLight;
uniform PositionalLight  gPositionalLight;
uniform float gSpecularIntensity, gSpecularPower;
uniform vec3 gEyeWorldPosition;
in vec3 Normal0;
in vec3 WorldPos0;





vec4 CalculateDirectionalLight()
{
	vec4 DiffuseColor,SpecularColor;
	vec3 Normal=normalize(Normal0);
	vec4 AmbientColor=vec4(gDirectionalLight.Color*gDirectionalLight.AmbientIntensity,1.0);
	float DiffuseFactor=dot(Normal,-gDirectionalLight.Direction);
	if(DiffuseFactor>0)																//angle is between -90 to 90.
	{
		DiffuseColor=vec4(gDirectionalLight.Color,1.0f)*gDirectionalLight.DiffuseIntensity*DiffuseFactor;
		vec3 VertexToEye=normalize(gEyeWorldPosition-WorldPos0);
		vec3 ReflectedLight=normalize(reflect(gDirectionalLight.Direction,Normal));
		float SpecularFactor=dot(VertexToEye,ReflectedLight);
		if(SpecularFactor>0)
		{
			SpecularFactor=pow(SpecularFactor,gSpecularPower);
			SpecularColor= vec4(gDirectionalLight.Color*gSpecularIntensity*SpecularFactor,1.0f);
		}
	}
	else
	{
		DiffuseColor=vec4(0,0,0,0);
		SpecularColor=vec4(0,0,0,0);
	}
	return(AmbientColor+DiffuseColor+SpecularColor);
}

vec4  CalculatePositionalLight()
{
	vec3 LightDirection=WorldPos0-gPositionalLight.Position;
	float distance=length(LightDirection);
	LightDirection=normalize(LightDirection);

	vec4 DiffuseColor,SpecularColor;
	vec3 Normal=normalize(Normal0);
	vec4 AmbientColor=vec4(gPositionalLight.Color*gPositionalLight.AmbientIntensity,1.0);
	float DiffuseFactor=dot(Normal,-LightDirection);
	if(DiffuseFactor>0)																//angle is between -90 to 90.
	{
		DiffuseColor=vec4(gPositionalLight.Color,1.0f)*gPositionalLight.DiffuseIntensity*DiffuseFactor;
		vec3 VertexToEye=normalize(gEyeWorldPosition-WorldPos0);
		vec3 ReflectedLight=normalize(reflect(LightDirection,Normal));
		float SpecularFactor=dot(VertexToEye,ReflectedLight);
		if(SpecularFactor>0)
		{
			SpecularFactor=pow(SpecularFactor,gSpecularPower);
			SpecularColor= vec4(gPositionalLight.Color*gSpecularIntensity*SpecularFactor,1.0f);
		}
	}
	else
	{
		DiffuseColor=vec4(0,0,0,0);
		SpecularColor=vec4(0,0,0,0);
	}

	vec4 TotalColor=(AmbientColor+DiffuseColor+SpecularColor);
	float Attenuation=gPositionalLight.Attenuation.Constant+(gPositionalLight.Attenuation.Linear*distance)+(gPositionalLight.Attenuation.Exponential*distance*distance);
	return TotalColor/Attenuation;
}


void main()
{
	vec4 TotalLight=CalculateDirectionalLight()+CalculatePositionalLight();
    FragColor = vec4(Color0, 1.0)*TotalLight;
}
