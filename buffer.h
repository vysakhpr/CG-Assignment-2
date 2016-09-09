#ifndef BUFFER_H
#define BUFFER_H

#include "math_utils.h"
OffModel *protein;
struct BoundBox
{
	Vector3f Center;
	float XWidth;
	float YWidth;
	float ZWidth;
}boundBox;



static void  CreateBuffers(char* OffFile)
{
	float maxBoundX=0,maxBoundY=0,maxBoundZ=0,minBoundX=0,minBoundY=0,minBoundZ=0;
	float x,y,z;
	float dX,dY,dZ,BoundBoxWidth;
	int i,j;
	protein=readOffFile(OffFile);


	for (i = 0; i < protein->numberOfVertices; i++)
	 {
	 	x=(protein->vertices[i]).x;
	 	y=(protein->vertices[i]).y;
	 	z=(protein->vertices[i]).z;
	 	if(i==0)
	 	{
	 		maxBoundX=x;maxBoundY=y;maxBoundZ=z;
	 		minBoundX=x;minBoundY=y;minBoundZ=z;
	 	}
	 	else
	 	{
	 		if(x>maxBoundX)
	 			maxBoundX=x;
	 		else if(x<minBoundX)
	 			minBoundX=x;

	 		if(y>maxBoundY)
	 			maxBoundY=y;
	 		else if(y<minBoundY)
	 			minBoundY=y;

	 		if(z>maxBoundZ)
	 			maxBoundZ=z;
	 		else if(z<minBoundZ)
	 			minBoundZ=z; 
	 	}
	 }
	dX=maxBoundX- minBoundX;
	dY=maxBoundY- minBoundY;
	dZ=maxBoundZ- minBoundZ;
	
	if(dX>dY)
	{
		if(dX>dZ)
			BoundBoxWidth=dX;
		else
			BoundBoxWidth=dZ;
	}
	else
	{
		if(dY>dZ)
			BoundBoxWidth=dY;
		else
			BoundBoxWidth=dZ;
	}
	if(BoundBoxWidth==0)
		BoundBoxWidth=1;
	//Scale = 1.0/BoundBoxWidth; 
	Scale=1;
	boundBox.Center.x=Scale*(maxBoundX-(dX/2));
	boundBox.Center.y=Scale*(maxBoundY-(dY/2));
	boundBox.Center.z=Scale*(maxBoundZ-(dZ/2));
	boundBox.XWidth=Scale*dX;
	boundBox.YWidth=Scale*dY;
	boundBox.ZWidth=Scale*dZ;
	for (i = 0; i < protein->numberOfVertices; i++)
	 {
	 	(protein->vertices[i]).x*=Scale;
	 	(protein->vertices[i]).y*=Scale;
	 	(protein->vertices[i]).z*=Scale;
	 	(protein->vertices[i]).x-=boundBox.Center.x;
	 	(protein->vertices[i]).y-=boundBox.Center.y;
	 	(protein->vertices[i]).z-=boundBox.Center.z;
	 }
	
	int* Indices = NULL;
	Indices=new int[protein->numberOfTriangles*3];
	if (Indices==NULL){
		cout << "Error: Memory couldnot be allocated"<< endl;
	 	exit(1);
	}
	for (i=0,j=0;i < protein->numberOfTriangles; i++,j+=3)
	{
	 	Indices[j]=(protein->triangles[i]).v[0];
	 	Indices[j+1]=(protein->triangles[i]).v[1];
	 	Indices[j+2]=(protein->triangles[i]).v[2];
	 	Vector3f v0=Vector3f((protein->vertices[Indices[j]]).x,(protein->vertices[Indices[j]]).y,(protein->vertices[Indices[j]]).z);
	 	Vector3f v1=Vector3f((protein->vertices[Indices[j+1]]).x,(protein->vertices[Indices[j+1]]).y,(protein->vertices[Indices[j+1]]).z);
	 	Vector3f v2=Vector3f((protein->vertices[Indices[j+2]]).x,(protein->vertices[Indices[j+2]]).y,(protein->vertices[Indices[j+2]]).z);
	 	Vector3f v10=v1-v0;
	 	Vector3f v20=v2-v0;
	 	Vector3f Normal=v10.Cross(v20);
	 	Normal.Normalize();
	 	(protein->vertices[Indices[j]]).Normal+=Normal;
	 	(protein->vertices[Indices[j+1]]).Normal+=Normal;
	 	(protein->vertices[Indices[j+1]]).Normal+=Normal;
	}

	for (i = 0; i < protein->numberOfVertices; i++)
	{
		(protein->vertices[i]).Normal.Normalize();
	}



	glGenBuffers(1,&IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int)*(protein->numberOfTriangles)*3, Indices, GL_STATIC_DRAW);

	glGenBuffers(1,&VBO);
	glBindBuffer(GL_ARRAY_BUFFER,VBO);
	glBufferData(GL_ARRAY_BUFFER,sizeof(Vertex)*(protein->numberOfVertices), protein->vertices, GL_STATIC_DRAW);


	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)(sizeof(float)*3));
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);


	//glEnableVertexAttribArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,IBO);
	//glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	FreeOffModel(protein);
}

#endif