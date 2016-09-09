#version 330

layout (location = 0) in vec3 Position;
layout (location = 1) in vec3 Normal;

uniform mat4 gWorld,gWVP;


out vec3 Normal0;
out vec3 WorldPos0;

void main()
{

    gl_Position=gWVP*vec4(Position,1.0);
    Normal0 = (transpose(inverse(gWorld)) * vec4(Normal, 0.0)).xyz;
    WorldPos0 = (transpose(inverse(gWorld))* vec4(Position, 0.0)).xyz;
}