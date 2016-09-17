#ifndef PATCH_H
#define PATCH_H

#include "math_utils.h"
#include "protein.h"
#include "helper.h"
class Patch
{
	GLuint IBO;
	int *Indices;
	int numIndex;
	int patchNum;
	Vector3f Color;
	int ExplodeVectorIndex;
public:
	Patch()
	{
		numIndex=0;
	}
	void Init(int x,int patch_num)
	{
		Indices=new int[x];
		patchNum=patch_num;
	}

	void AddTriangle(int x)
	{	
		Indices[numIndex++]=x;
	}

	int GetIndexValue(int i)
	{
		return Indices[i];
	}

	int GetNumIndex()
	{
		return numIndex;
	}

	void SetPatchNum(int x)
	{
		patchNum=x;
	}
	
	void SetColor(float x, float y, float z)
	{

		Color=Vector3f(x/256,y/256,z/256);
	}

	void GenBuffer()
	{
		ExplodeVectorIndex=Indices[numIndex-1];
		glGenBuffers(1,&IBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,IBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int)*(numIndex), Indices, GL_STATIC_DRAW);
	}

	int GetExplodeVectorIndex()
	{
		return ExplodeVectorIndex;
	}

	void RenderPatch()
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,IBO);
		glDrawElements(GL_TRIANGLES,numIndex,GL_UNSIGNED_INT,0);
	}

	void RenderColor()
	{
		//glUniform3f(ChainColorLocation,1.0,1.0,1.0);
		//Color.Print();
		glUniform3f(ChainColorLocation,Color.x,Color.y,Color.z);
	}
	void Print()
	{
		for (int i = 0; i < numIndex; ++i)
		{
			cout<<Indices[i]<<endl;
		}
	}

};

#endif