#version 330

layout	(triangles) in;
layout	(triangle_strip,max_vertices=3) out;
in vec3 vNormal0[];
out vec3 Normal0;

void main()
{
	for(int i=0;i<gl_in.length();i++)
	{
		gl_Position=gl_in[i].gl_Position;
		Normal0=vNormal0[i];
		EmitVertex();
	}
	EndPrimitive();
}