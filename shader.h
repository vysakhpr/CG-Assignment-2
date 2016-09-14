#ifndef SHADER_H
#define SHADER_H

#include "lighting.h"
#include "camera.h"

GLuint gWorldLocation,gWVPLocation,DLightColorLocation,DLightAmbientIntensityLocation,DLightDirectionLocation,DLightDiffuseIntensityLocation;
GLuint gEyeWorldPositionLocation,SpecularIntensityLocation,SpecularPowerLocation;
GLuint PLightColorLocation, PLightAmbientIntensityLocation, PLightDiffuseIntensityLocation, PLightPositionLocation;
GLuint PLightAttenuationConstantLocation,PLightAttenuationLinearLocation,PLightAttenuationExpLocation;
GLuint LigandFlagLocation, LigandTransLocation;

static void AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType) {
	GLuint ShaderObj = glCreateShader(ShaderType);

	if (ShaderObj == 0) {
		fprintf(stderr, "Error creating shader type %d\n", ShaderType);
		exit(0);
	}

	const GLchar * p[1];
	p[0] = pShaderText;
	GLint Lengths[1];
	Lengths[0] = strlen(pShaderText);
	glShaderSource(ShaderObj, 1, p, Lengths);
	glCompileShader(ShaderObj);
	GLint success;
	glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);
	if (!success) {
		GLchar InfoLog[1024];
		glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
		fprintf(stderr, "Error compiling shader type %d: '%s'\n", ShaderType, InfoLog);
		exit(1);
	}

	glAttachShader(ShaderProgram, ShaderObj);
}

using namespace std;

static void CompileShaders() {

	GLuint ShaderProgram = glCreateProgram();

	if (ShaderProgram == 0) {
		fprintf(stderr, "Error creating shader program\n");
		exit(1);
	}

	string vs, fs, gs;

	if (!ReadFile(pVSFileName, vs)) {
		exit(1);
	}

	if (!ReadFile(pFSFileName, fs)) {
		exit(1);
	}

	if (!ReadFile(pGSFileName, gs)) {
		exit(1);
	}

	AddShader(ShaderProgram, vs.c_str(), GL_VERTEX_SHADER);
	AddShader(ShaderProgram, fs.c_str(), GL_FRAGMENT_SHADER);
	//AddShader(ShaderProgram, gs.c_str(), GL_GEOMETRY_SHADER);

	GLint Success = 0;
	GLchar ErrorLog[1024] = {0};

	glLinkProgram(ShaderProgram);
	
	glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &Success);
	if (Success == 0) {
		glGetProgramInfoLog(ShaderProgram, sizeof (ErrorLog), NULL, ErrorLog);
		fprintf(stderr, "Error linking shader program: '%s'\n", ErrorLog);
		exit(1);
	}

	glValidateProgram(ShaderProgram);
	glGetProgramiv(ShaderProgram, GL_VALIDATE_STATUS, &Success);
	if (!Success) {
		glGetProgramInfoLog(ShaderProgram, sizeof (ErrorLog), NULL, ErrorLog);
		fprintf(stderr, "Invalid shader program: '%s'\n", ErrorLog);
		exit(1);
	}

	glUseProgram(ShaderProgram);
	gWVPLocation=glGetUniformLocation(ShaderProgram,"gWVP");
	gWorldLocation=glGetUniformLocation(ShaderProgram,"gWorld");
	DLightColorLocation=glGetUniformLocation(ShaderProgram,"gDirectionalLight.Color");
	DLightAmbientIntensityLocation=glGetUniformLocation(ShaderProgram,"gDirectionalLight.AmbientIntensity");
	DLightDirectionLocation=glGetUniformLocation(ShaderProgram,"gDirectionalLight.Direction");
	DLightDiffuseIntensityLocation=glGetUniformLocation(ShaderProgram,"gDirectionalLight.DiffuseIntensity");
	gEyeWorldPositionLocation=glGetUniformLocation(ShaderProgram,"gEyeWorldPosition");
	SpecularIntensityLocation=glGetUniformLocation(ShaderProgram,"gSpecularIntensity");
	SpecularPowerLocation=glGetUniformLocation(ShaderProgram,"gSpecularPower");
	PLightPositionLocation=glGetUniformLocation(ShaderProgram,"gPositionalLight.Position");
	PLightColorLocation=glGetUniformLocation(ShaderProgram,"gPositionalLight.Color");
	PLightAmbientIntensityLocation=glGetUniformLocation(ShaderProgram,"gPositionalLight.AmbientIntensity");
	PLightDiffuseIntensityLocation=glGetUniformLocation(ShaderProgram,"gPositionalLight.DiffuseIntensity");
	PLightAttenuationConstantLocation=glGetUniformLocation(ShaderProgram,"gPositionalLight.Attenuation.Constant");
	PLightAttenuationLinearLocation=glGetUniformLocation(ShaderProgram,"gPositionalLight.Attenuation.Linear");
	PLightAttenuationExpLocation=glGetUniformLocation(ShaderProgram,"gPositionalLight.Attenuation.Exponential");
	LigandFlagLocation=glGetUniformLocation(ShaderProgram,"gLigandFlag");
	LigandTransLocation=glGetUniformLocation(ShaderProgram,"gLigandTrans");
	if(	gWVPLocation==0xFFFFFFFF || gWorldLocation==0xFFFFFFFF || DLightColorLocation==0xFFFFFFFF ||
		DLightAmbientIntensityLocation==0xFFFFFFFF || DLightDirectionLocation == 0xFFFFFFFF ||
		DLightDiffuseIntensityLocation == 0xFFFFFFFF || gEyeWorldPositionLocation == 0xFFFFFFFF ||
		SpecularIntensityLocation == 0xFFFFFFFF || SpecularPowerLocation == 0xFFFFFFFF || PLightColorLocation == 0xFFFFFFFF ||
		PLightPositionLocation == 0xFFFFFFFF || PLightAmbientIntensityLocation == 0xFFFFFFFF || PLightDiffuseIntensityLocation 
		 == 0xFFFFFFFF || PLightAttenuationConstantLocation == 0xFFFFFFFF || PLightAttenuationLinearLocation == 0xFFFFFFFF || 
		 PLightAttenuationExpLocation == 0xFFFFFFFF || LigandTransLocation == 0xFFFFFFFF || LigandFlagLocation == 0xFFFFFFFF)
	{
		cout<<"Location Cannot be found"<<endl;
		exit(1);
	}
	glUniform1i(LigandFlagLocation,0);
}


void SetLightsInShader(Lighting lights, Camera cam)
{
	DirectionalLight DLight=lights.GetDirectionalLight();
	glUniform3f(DLightColorLocation, DLight.Color.x, DLight.Color.y, DLight.Color.z);
    glUniform1f(DLightAmbientIntensityLocation, DLight.AmbientIntensity);
    Vector3f Direction=DLight.Direction;
    Direction.Normalize();
    glUniform3f(DLightDirectionLocation,Direction.x,Direction.y,Direction.z);
    glUniform1f(DLightDiffuseIntensityLocation,DLight.DiffuseIntensity);
    glUniform3f(gEyeWorldPositionLocation,cam.GetPosition().x,cam.GetPosition().y,cam.GetPosition().z);
    glUniform1f(SpecularIntensityLocation,1.0f);
    glUniform1f(SpecularPowerLocation,32);
    PositionalLight PLight=lights.GetPositionalLight();
    glUniform3f(PLightColorLocation,PLight.Color.x,PLight.Color.y,PLight.Color.z);
    glUniform3f(PLightPositionLocation,PLight.Position.x,PLight.Position.y,PLight.Position.z);
    glUniform1f(PLightDiffuseIntensityLocation,PLight.DiffuseIntensity);
    glUniform1f(PLightAmbientIntensityLocation, PLight.AmbientIntensity);
    glUniform1f(PLightAttenuationConstantLocation,PLight.Attenuation.Constant);
    glUniform1f(PLightAttenuationLinearLocation,PLight.Attenuation.Linear);
    glUniform1f(PLightAttenuationExpLocation,PLight.Attenuation.Exponential);

}


#endif