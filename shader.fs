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
in vec3 Normal0;



void main()
{
	vec4 DiffuseColor;
	vec4 AmbientColor=vec4(gDirectionalLight.Color*gDirectionalLight.AmbientIntensity,1.0);
	float DiffuseFactor=dot(normalize(Normal0),-gDirectionalLight.Direction);
	if(DiffuseFactor>0)																//angle is between -90 to 90.
		DiffuseColor=vec4(gDirectionalLight.Color*gDirectionalLight.DiffuseIntensity*DiffuseFactor,1.0f);
	else
		DiffuseColor=vec4(0,0,0,0);
    FragColor = vec4(0.0, 1.0, 0.0, 1.0)*(AmbientColor+DiffuseColor);
}
