#version 330

layout	(triangles) in;
layout	(triangle_strip,max_vertices=3) out;
in vec3 vNormal0[];
in vec3 vColor0[];
in vec3 vWorldPos0[];
out vec3 Normal0;
out vec3 Color0;
out vec3 WorldPos0;
void main()
{
	for(int i=0;i<gl_in.length();i++)
	{
		gl_Position=gl_in[i].gl_Position;
		Normal0=vNormal0[i];
		WorldPos0=vWorldPos0[i];
		Color0=vColor0[i];
		EmitVertex();
	}
	EndPrimitive();
}