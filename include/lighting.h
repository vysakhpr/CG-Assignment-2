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

struct PositionalLight
{	
	Vector3f Color;
	Vector3f Position;
	float AmbientIntensity;
	float DiffuseIntensity;
	struct 
	{
		float Constant;
		float Linear;
		float Exponential;
	}Attenuation;
};

class Lighting
{
	DirectionalLight DLight;
	PositionalLight PLight;
	int DLightFlag,PLightFlag;
public:
	Lighting()
	{
		DLight.Color=Vector3f(1.0,1.0,1.0);
		DLight.AmbientIntensity=0.1f;
		DLight.Direction=Vector3f(1.0,0.0,0.0);
		DLight.DiffuseIntensity=0.75f;
		PLight.Color=Vector3f(1.0,1.0,1.0);
		PLight.Position=Vector3f(0.0,0.0,0.0);
		PLight.AmbientIntensity=0.0f;
		PLight.DiffuseIntensity=0.6f;
		PLight.Attenuation.Constant=1.0f;
		PLight.Attenuation.Linear=0.1f;
		PLight.Attenuation.Exponential=0.001f;
		DLightFlag=1;
		PLightFlag=1;
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
		DirectionalLight Light=DLight;
		Light.AmbientIntensity*=DLightFlag;
		Light.DiffuseIntensity*=DLightFlag;
		Light.Color*=DLightFlag;
		return Light;
	}

	void SetPositionalLightPosition(Vector3f c)
	{
		PLight.Position=c;
	}
	PositionalLight GetPositionalLight()
	{
		PositionalLight Light=PLight;
		Light.AmbientIntensity*=PLightFlag;
		Light.DiffuseIntensity*=PLightFlag;
		Light.Color*=PLightFlag;
		return Light;
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

	void ToggleDirectionalLightSwitch()
	{
		DLightFlag=!(DLightFlag);
	}

	void TogglePositionalLightSwitch()
	{
		PLightFlag=!(PLightFlag);
	}


};
#endif