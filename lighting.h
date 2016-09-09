#ifndef LIGHTING_H
#define LIGHTING_H

#include "math_utils.h"

struct DirectionalLight
{
	Vector3f Color;
	float AmbientIntensity;
	Vector3f Direction;
	float DiffuseIntensity;
};

class Lighting
{
	DirectionalLight DLight;
public:
	Lighting()
	{
		DLight.Color=Vector3f(1.0,1.0,1.0);
		DLight.AmbientIntensity=0.1f;
		DLight.Direction=Vector3f(1.0,0.0,0.0);
		DLight.DiffuseIntensity=0.75f;
	}




	void SetDirectionalLightColor(Vector3f c)
	{
		DLight.Color=c;
	}
	void SetDirectionalLightAmbientIntensity(float c)
	{
		DLight.AmbientIntensity=c;
	}
	DirectionalLight GetDirectionalLight()
	{
		return DLight;
	}
	void IncreaseDirectionalLightAmbientIntensity()
	{
		DLight.AmbientIntensity+=0.1;
		if(DLight.AmbientIntensity<0)
			DLight.AmbientIntensity=0;
		else if(DLight.AmbientIntensity>1)
			DLight.AmbientIntensity=1;
	}
	void DecreaseDirectionalLightAmbientIntensity()
	{
		DLight.AmbientIntensity-=0.1;
		if(DLight.AmbientIntensity<0)
			DLight.AmbientIntensity=0;
		else if(DLight.AmbientIntensity>1)
			DLight.AmbientIntensity=1;
	}
	void SwitchDirectionalLightOn()
	{
		DLight.AmbientIntensity=0.1f;
		DLight.DiffuseIntensity=0.75f;
	}

	void SwitchDirectionalLightOff()
	{
		DLight.AmbientIntensity=0.0f;
		DLight.DiffuseIntensity=0.0f;
	}


};
#endif