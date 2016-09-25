#include <iostream>
#include <fstream>
#include "helper.h"
#include "file_utils.h"
#include "math_utils.h"

typedef struct Ed{
	int chainID;
	bool f;

	Ed()
	{
		chainID=0;
		f=false;
	}
}edge;

int main(int argc, char** argv)
{
	char* OffFile="protein_database/4hhb.off";
	OffModel *off=readOffFile(OffFile);
	edge* EdgeFlags=new edge[off->numberOfVertices];	
	for (int i = 0; i < off->numberOfVertices; ++i)
	{
		EdgeFlags[i].f=false;
	}

	int count=0;
	for (int i = 0; i < off->numberOfTriangles; ++i)
		{
			for(int j=0;j<3;j++)
			{
				if(EdgeFlags[off->triangles[i].v[j]].f)
				{
					count++;
				}
				else
				{
					if(EdgeFlags[off->triangles[i].v[j]].chainID==0){
						EdgeFlags[off->triangles[i].v[j]].chainID=off->triangles[i].chainID;
					}
					else
					{
						if(EdgeFlags[off->triangles[i].v[j]].chainID!=off->triangles[i].chainID)
						{
							EdgeFlags[off->triangles[i].v[j]].f=true;
						}
					}
				}
			}
		}

}
