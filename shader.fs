#version 330

out vec4 FragColor;
struct DirectionalLight
{
	vec3 Color;
	float AmbientIntensity;
	vec3 Direction;
	float DiffuseIntensity;
};

uniform DirectionalLight gDirectionalLight;
uniform float gSpecularIntensity, gSpecularPower;
uniform vec3 gEyeWorldPosition;
in vec3 Normal0;
in vec3 WorldPos0;



void main()
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

    FragColor = vec4(0.0, 1.0, 0.0, 1.0)*(AmbientColor+DiffuseColor+SpecularColor);
}
