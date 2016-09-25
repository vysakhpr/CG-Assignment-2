#version 330

layout (location = 0) in vec3 Position;
layout (location = 1) in vec3 Normal;
layout (location = 2) in vec3 Color;

uniform mat4 gWorld,gWVP,gLigandTrans;
uniform int gLigandFlag;
uniform int gExplodeFlag;
uniform mat4 gExplodeTranslate;
uniform int gEdgeFlag;


out vec3 Normal0;
out vec3 WorldPos0;
out vec3 Color0;

void main()
{
	if(gLigandFlag)
	{
		gl_Position=gWVP*gLigandTrans*vec4(Position,1.0);
    	Normal0 = (transpose(inverse(gWorld*gLigandTrans)) * vec4(Normal, 0.0)).xyz;
    	WorldPos0 = (transpose(inverse(gWorld*gLigandTrans))* vec4(Position, 0.0)).xyz;
	}    	
    else
    {
        if(gExplodeFlag)
        {
            gl_Position=gWVP*gExplodeTranslate*vec4(Position,1.0);
            Normal0 = (transpose(inverse(gWorld)) * vec4(Normal, 0.0)).xyz;
            WorldPos0 = (transpose(inverse(gWorld))* vec4(Position, 0.0)).xyz;
        }
        else
        {
    	   gl_Position=gWVP*vec4(Position,1.0);
    	   Normal0 = (transpose(inverse(gWorld)) * vec4(Normal, 0.0)).xyz;
    	   WorldPos0 = (transpose(inverse(gWorld))* vec4(Position, 0.0)).xyz;
        }
    }
    Color0=Color;
    if(gEdgeFlag)
    {
        Color0=vec3(1.0,1.0,1.0);
    }
    
}