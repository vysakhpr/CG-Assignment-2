#ifndef PROTEIN_H
#define PROTEIN_H

#include "math_utils.h"
#include "helper.h"
#include "patch.h"

class ProteinMolecule
{	
public:
	GLuint VBO,IBO,VAO,CHAIN_CBO,AMINO_CBO,CBO;
	BoundBox boundBox;
	OffModel *protein;
	Vector3f *Colors;
	Patch *patches,*aminos;
	int patchno,amino_no;
	bool active_amino;
	ProteinMolecule()
	{
		
	}
	
	void ToggleColoring()
	{
		active_amino=!active_amino;
	}

	void LoadBuffers(char* OffFile)
	{
		float maxBoundX=0,maxBoundY=0,maxBoundZ=0,minBoundX=0,minBoundY=0,minBoundZ=0;
		patchno=1;
		amino_no=1;
		float x,y,z;
		float dX,dY,dZ,BoundBoxWidth;
		int i,j;
		protein=readOffFile(OffFile);
		Colors=new Vector3f[protein->numberOfVertices];
		active_amino=false;

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
		boundBox.Center.x=(maxBoundX-(dX/2));
		boundBox.Center.y=(maxBoundY-(dY/2));
		boundBox.Center.z=(maxBoundZ-(dZ/2));
		boundBox.XWidth=dX;
		boundBox.YWidth=dY;
		boundBox.ZWidth=dZ;
		for (i = 0; i < protein->numberOfVertices; i++)
	 	{
		 	(protein->vertices[i]).x-=boundBox.Center.x;
	 		(protein->vertices[i]).y-=boundBox.Center.y;
	 		(protein->vertices[i]).z-=boundBox.Center.z;
	 	}
	
		int* Indices = NULL,*EdgeIndices=NULL;
		Indices=new int[protein->numberOfTriangles*3];
		EdgeIndices=new int[protein->numberOfTriangles*3];
		if (Indices==NULL){
			cout << "Error: Memory couldnot be allocated"<< endl;
	 		exit(1);
		}
		for (i=0,j=0;i < protein->numberOfTriangles; i++,j+=3)
		{
			if((protein->triangles[i]).chainID>patchno)
			{
				patchno=(protein->triangles[i]).chainID;
			}

			if((protein->triangles[i]).aminoType>amino_no)
			{
				amino_no=(protein->triangles[i]).aminoType;
			}

	 		Indices[j]=(protein->triangles[i]).v[0];
	 		Indices[j+1]=(protein->triangles[i]).v[1];
	 		Indices[j+2]=(protein->triangles[i]).v[2];
	 		Vector3f v0=Vector3f((protein->vertices[Indices[j]]).x,(protein->vertices[Indices[j]]).y,(protein->vertices[Indices[j]]).z);
	 		Vector3f v1=Vector3f((protein->vertices[Indices[j+1]]).x,(protein->vertices[Indices[j+1]]).y,(protein->vertices[Indices[j+1]]).z);
	 		Vector3f v2=Vector3f((protein->vertices[Indices[j+2]]).x,(protein->vertices[Indices[j+2]]).y,(protein->vertices[Indices[j+2]]).z);
	 		Vector3f v10=v1-v0;
	 		Vector3f v20=v2-v0;
	 		Vector3f v21=v2-v1;
	 		Vector3f Normal=v10.Cross(v20);
	 		float Area= Normal.length()/2;
	 		v10.Normalize();
	 		v20.Normalize();
	 		v21.Normalize();
	 		Normal.Normalize();
	 		(protein->vertices[Indices[j]]).Normal+=Normal*Area*ToDegree(acos(v10.Dot(v20)));
	 		(protein->vertices[Indices[j+1]]).Normal+=Normal*Area*ToDegree(acos((v10*-1.0).Dot(v21)));
	 		(protein->vertices[Indices[j+2]]).Normal+=Normal*Area*ToDegree(acos((v20*-1.0).Dot((v21*-1.0))));
		}

		for (i = 0; i < protein->numberOfVertices; i++)
		{
			(protein->vertices[i]).Normal.Normalize();
			Colors[i]=Vector3f(0.0,1.0,0.0);
		}

		patches=new Patch[patchno];
		aminos=new Patch[(amino_no)];
		cout<<amino_no<<endl;

		for(i=0;i<patchno;i++)
		{
			patches[i].Init(protein->numberOfTriangles*3,i+1);
		}

		for(i=0;i<amino_no;i++)
		{
			aminos[i].Init(protein->numberOfTriangles*3,i+1);
		}

		for(i=0;i<protein->numberOfTriangles;i++)
		{	
			
			patches[protein->triangles[i].chainID-1].AddTriangle(protein->triangles[i].v[0]);
			patches[protein->triangles[i].chainID-1].AddTriangle(protein->triangles[i].v[1]);
			patches[protein->triangles[i].chainID-1].AddTriangle(protein->triangles[i].v[2]);
		}

		for(i=0;i<protein->numberOfTriangles;i++)
		{	
			
			aminos[protein->triangles[i].aminoType-1].AddTriangle(protein->triangles[i].v[0]);
			aminos[protein->triangles[i].aminoType-1].AddTriangle(protein->triangles[i].v[1]);
			aminos[protein->triangles[i].aminoType-1].AddTriangle(protein->triangles[i].v[2]);
		}


		//patches[0].Print();
		for (i = 0; i < patchno; ++i)
		{
			patches[i].GenBuffer();
			patches[i].SetColor(aminoColors[i][0],aminoColors[i][1],aminoColors[i][2]);
		}

		for (i = 0; i < amino_no; ++i)
		{
			aminos[i].GenBuffer();
			aminos[i].SetColor(aminoColors[i][0],aminoColors[i][1],aminoColors[i][2]);
		}

		
		glGenBuffers(1,&IBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,IBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int)*(protein->numberOfTriangles)*3, Indices, GL_STATIC_DRAW);
		glGenBuffers(1,&VBO);
		glBindBuffer(GL_ARRAY_BUFFER,VBO);
		glBufferData(GL_ARRAY_BUFFER,sizeof(Vertex)*(protein->numberOfVertices), protein->vertices, GL_STATIC_DRAW);

		glGenBuffers(1,&CBO);
		glBindBuffer(GL_ARRAY_BUFFER,CBO);
		glBufferData(GL_ARRAY_BUFFER,sizeof(float)*3*(protein->numberOfVertices), Colors, GL_STATIC_DRAW);
	

		glGenBuffers(1,&CHAIN_CBO);
		glBindBuffer(GL_ARRAY_BUFFER,CHAIN_CBO);
		glBufferData(GL_ARRAY_BUFFER,sizeof(float)*3*(protein->numberOfVertices), Colors, GL_STATIC_DRAW);
	
		for (i = 0; i < protein->numberOfVertices; i++)
		{
			Colors[i]=Vector3f(0.0,1.0,0.0);
		}		
		glGenBuffers(1,&AMINO_CBO);
		glBindBuffer(GL_ARRAY_BUFFER,AMINO_CBO);
		glBufferData(GL_ARRAY_BUFFER,sizeof(float)*3*(protein->numberOfVertices), Colors, GL_STATIC_DRAW);


		//glEnableVertexAttribArray(0);
		//glEnableVertexAttribArray(1);
		
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		//FreeOffModel(protein);
	}


	void RenderDisplay()
	{
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)(sizeof(float)*3));
		glBindBuffer(GL_ARRAY_BUFFER, CBO);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3f), 0);
		//glDisableVertexAttribArray(1);
		//glDisableVertexAttribArray(0);


		//glEnableVertexAttribArray(0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,IBO);
		glBindVertexArray(0);

		
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glBindVertexArray(VAO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,IBO);
		glBindBuffer(GL_ARRAY_BUFFER,VBO);
	//glDrawElements(GL_TRIANGLES,protein->numberOfTriangles*3,GL_UNSIGNED_INT,0);
		if(active_amino)
		{
			for (int i = 0; i < amino_no; ++i)
			{
				Matrix4f ExplodeTrans;
				ExplodeTrans.InitTranslationTransform(0,0,0);
			
				glUniformMatrix4fv(ExplodeTranslateLocation,1,GL_TRUE,&ExplodeTrans.m[0][0]);
				aminos[i].RenderColor();
				aminos[i].RenderPatch();
			}
		
		}
		else
		{
			for (int i = 0; i < patchno; ++i)
			{
				Vector3f v=PatchCenter(patches[i]);
				v.Normalize();
				v=v*10;
				Matrix4f ExplodeTrans;
				ExplodeTrans.InitTranslationTransform(v.x,v.y,v.z);
			
				glUniformMatrix4fv(ExplodeTranslateLocation,1,GL_TRUE,&ExplodeTrans.m[0][0]);
				patches[i].RenderColor();
				patches[i].RenderPatch();
			}
		}

		glBindVertexArray(0);
		glDisableVertexAttribArray(2);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(0);
	
	}




	Vector3f PatchCenter(Patch &p)
	{
		float maxBoundX=0,maxBoundY=0,maxBoundZ=0,minBoundX=0,minBoundY=0,minBoundZ=0;
		float x,y,z;
		float dX,dY,dZ;
		int i;
		Vector3f Center;
		for (i = 0; i < p.GetNumIndex(); i++)
	 	{
	 		x=(protein->vertices[p.GetIndexValue(i)]).x;
	 		y=(protein->vertices[p.GetIndexValue(i)]).y;
	 		z=(protein->vertices[p.GetIndexValue(i)]).z;
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
		Center.x=(maxBoundX-(dX/2));
		Center.y=(maxBoundY-(dY/2));
		Center.z=(maxBoundZ-(dZ/2));
		return Center;
	}

};

#endif