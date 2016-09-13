#ifndef LIGAND_H
#define LIGAND_H


#include "math_utils.h"
#include "helper.h"

class LigandMolecule
{
public:
	BoundBox boundBox;
	Molecule* ligand;
	Vector3f *sphereVerts,*VertexSpheres;
	Vector3f *sphereNormals,*NormalsSpheres;
	GLuint *sphereIndices,*IndexSpheres;
	int numSphereVerts,numVertexSpheres;
	int numSphereTris,numVertexTriangles;
	Vector3f *cylinVerts,*VertexCylinders;
	Vector3f *cylinNormals,*NormalsCylinders;
	GLuint *cylinIndices,*IndexCylinders;
	int numCylinVerts;
	int numCylinTris;
	int TotalSphereVertex,TotalCylinderVertex;
	int TotalSphereIndex,TotalCylinderIndex;
		
	GLuint SPH_VBO, SPH_NORM_VBO, SPH_IBO, VAO,CYL_VBO, CYL_NORM_VBO,CYL_IBO;


	LigandMolecule()
	{
		sphereVerts = NULL;
		sphereIndices = NULL;
		numSphereVerts = 0;
		numSphereTris = 0;
		cylinVerts = NULL;
		cylinNormals = NULL;
		cylinIndices = NULL;
		numCylinVerts = 0;
		numCylinTris = 0;
	
	}
	

	void LoadBuffers(char* crdFile)
	{
		int i;
		int slices,stacks;
		slices=80;
		stacks=80;
		ligand=readCrdFile(crdFile);
		TotalSphereVertex=ligand->numAtoms*(slices * (stacks-1) + 2);
		TotalSphereIndex=ligand->numAtoms*((slices * (stacks-2) * 2 + 2 * slices)*3);
		VertexSpheres=new Vector3f[TotalSphereVertex];
		NormalsSpheres=new Vector3f[TotalSphereVertex];
		IndexSpheres=new GLuint[TotalSphereIndex];
		for(int k=0;k<ligand->numAtoms;k++){

			UnitSphere(slices,stacks);

			for(i=0;i<numSphereVerts;i++)
			{
				sphereVerts[i].x*=0.2*atomic_radii[(ligand->atoms[k]).type-1];
				sphereVerts[i].y*=0.2*atomic_radii[(ligand->atoms[k]).type-1];
				sphereVerts[i].z*=0.2*atomic_radii[(ligand->atoms[k]).type-1];
				sphereVerts[i].x+=ligand->atoms[k].x;
				sphereVerts[i].y+=ligand->atoms[k].y;
				sphereVerts[i].z+=ligand->atoms[k].z;
			}

			for (i = 0; i < numSphereVerts; ++i)
			{
				sphereNormals[i]=Vector3f(0.0,0.0,0.0);
			}

			for (i = 0; i < numSphereTris*3; i+=3)
			{
				Vector3f v0=Vector3f(sphereVerts[sphereIndices[i]].x,sphereVerts[sphereIndices[i]].y,sphereVerts[sphereIndices[i]].z);
		 		Vector3f v1=Vector3f(sphereVerts[sphereIndices[i+1]].x,sphereVerts[sphereIndices[i+1]].y,sphereVerts[sphereIndices[i+1]].z);
		 		Vector3f v2=Vector3f(sphereVerts[sphereIndices[i+2]].x,sphereVerts[sphereIndices[i+2]].y,sphereVerts[sphereIndices[i+2]].z);
	 			Vector3f v10=v1-v0;
		 		Vector3f v20=v2-v0;
		 		Vector3f v21=v2-v1;
	 			Vector3f Normal=v10.Cross(v20);
		 		float Area= Normal.length()/2;
		 		v10.Normalize();
	 			v20.Normalize();
		 		v21.Normalize();
		 		Normal.Normalize();
	 			sphereNormals[sphereIndices[i]]+=Normal*Area*ToDegree(acos(v10.Dot(v20)));
	 			sphereNormals[sphereIndices[i+1]]+=Normal*Area*ToDegree(acos((v10*-1.0).Dot(v21)));
	 			sphereNormals[sphereIndices[i+2]]+=Normal*Area*ToDegree(acos((v20*-1.0).Dot((v21*-1.0))));
			}
			for (i = 0; i < numSphereVerts; ++i)
			{
				sphereNormals[i].Normalize();
			}


			for (i = 0; i < numSphereVerts; ++i)
			{
				VertexSpheres[k*numSphereVerts+i]=sphereVerts[i];
				NormalsSpheres[k*numSphereVerts+i]=sphereNormals[i];
			}

			for (i = 0; i < numSphereTris*3; ++i)
			{
				IndexSpheres[k*numSphereTris*3+i]=k*numSphereVerts+sphereIndices[i];
			}


		}

		

		
		TotalCylinderVertex=ligand->numBonds*(slices * 2);
		TotalCylinderIndex=ligand->numBonds*((slices * 2)*3);
		VertexCylinders=new Vector3f[TotalCylinderVertex];
		NormalsCylinders=new Vector3f[TotalCylinderVertex];
		IndexCylinders=new GLuint[TotalCylinderIndex];
		
		
		for(int k=0;k<ligand->numBonds;k++)
		{
			
			Atom atom1=ligand->atoms[ligand->bonds[k].atom1];
			Atom atom2=ligand->atoms[ligand->bonds[k].atom2];
			UnitCylinder(slices,stacks);


			for (i = 0; i < numCylinVerts; i+=2)
			{
				cylinVerts[i].x+=atom1.x;
				cylinVerts[i].y+=atom1.y;
				cylinVerts[i].z+=atom1.z;
				cylinVerts[i+1].x+=atom2.x;
				cylinVerts[i+1].y+=atom2.y;
				cylinVerts[i+1].z+=atom2.z-1;
				
			}

			for (i = 0; i < numCylinVerts; ++i)
			{
				cylinNormals[i].Normalize();
			}
			for (i = 0; i < numCylinVerts; ++i)
			{
				VertexCylinders[k*numCylinVerts+i]=cylinVerts[i];
				NormalsCylinders[k*numCylinVerts+i]=cylinNormals[i];
			}

			for (i = 0; i < numCylinTris*3; ++i)
			{
				IndexCylinders[k*numCylinTris*3+i]=k*numCylinVerts+cylinIndices[i];
			}
		}
		
		for (i = 0; i < TotalSphereVertex; ++i)
		{
			Vector3f TranslateCoordinates=Vector3f(ligand->srcCoords[0],ligand->srcCoords[1],ligand->srcCoords[2]);
			Vector3f Distance= TranslateCoordinates-VertexSpheres[i];
			VertexSpheres[i]+=Distance*0.9;
		}

		for (i = 0; i < TotalCylinderVertex; ++i)
		{
			Vector3f TranslateCoordinates=Vector3f(ligand->srcCoords[0],ligand->srcCoords[1],ligand->srcCoords[2]);
			Vector3f Distance= TranslateCoordinates- VertexCylinders[i];
			VertexCylinders[i]+=Distance*0.9;
		}

		glBindVertexArray(VAO);
		glGenBuffers(1,&SPH_IBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,SPH_IBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*TotalSphereIndex, IndexSpheres, GL_STATIC_DRAW);
		glGenBuffers(1,&SPH_VBO);
		glBindBuffer(GL_ARRAY_BUFFER,SPH_VBO);
		glBufferData(GL_ARRAY_BUFFER,sizeof(float)*3*(TotalSphereVertex), VertexSpheres, GL_STATIC_DRAW);
		//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3f), 0);
		glGenBuffers(1,&SPH_NORM_VBO);
		glBindBuffer(GL_ARRAY_BUFFER,SPH_NORM_VBO);
		glBufferData(GL_ARRAY_BUFFER,sizeof(Vector3f)*(TotalSphereVertex), NormalsSpheres, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, SPH_NORM_VBO);
		//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3f), 0);


		glGenBuffers(1,&CYL_IBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,CYL_IBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*(TotalCylinderIndex), IndexCylinders, GL_STATIC_DRAW);
		glGenBuffers(1,&CYL_VBO);
		glBindBuffer(GL_ARRAY_BUFFER,CYL_VBO);
		glBufferData(GL_ARRAY_BUFFER,sizeof(float)*3*(TotalCylinderVertex), VertexCylinders, GL_STATIC_DRAW);
		//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3f), 0);
		glGenBuffers(1,&CYL_NORM_VBO);
		glBindBuffer(GL_ARRAY_BUFFER,CYL_NORM_VBO);
		glBufferData(GL_ARRAY_BUFFER,sizeof(Vector3f)*(TotalCylinderVertex),NormalsCylinders, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, CYL_NORM_VBO);




		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,SPH_IBO);
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		FreeMolecule(ligand);
	}


	void RenderDisplay()
	{
		
		RenderSphere();
		RenderCylinder();

		
	}

	void RenderSphere()
	{
		glBindBuffer(GL_ARRAY_BUFFER,SPH_VBO);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3f), 0);
		glBindBuffer(GL_ARRAY_BUFFER, SPH_NORM_VBO);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3f), 0);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER,SPH_VBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,SPH_IBO);
		glDrawElements(GL_TRIANGLES,TotalSphereIndex,GL_UNSIGNED_INT,0);
		glBindVertexArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(0);
		
	}

	void RenderCylinder()
	{
		glBindBuffer(GL_ARRAY_BUFFER,CYL_VBO);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3f), 0);
		glBindBuffer(GL_ARRAY_BUFFER, CYL_NORM_VBO);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3f), 0);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER,CYL_VBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,CYL_IBO);
		glDrawElements(GL_TRIANGLES,TotalCylinderIndex,GL_UNSIGNED_INT,0);
		glBindVertexArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(0);
	}




	void UnitSphere(int slices, int stacks){
		if (sphereVerts != NULL) free(sphereVerts);
		if (sphereIndices != NULL) free(sphereIndices);

		numSphereVerts = slices * (stacks-1) + 2;
		sphereVerts =  (Vector3f *) malloc(numSphereVerts * sizeof(Vector3f));
		sphereNormals= (Vector3f *) malloc(numSphereVerts * sizeof(Vector3f));

		int count = 0;
		for (int i=1; i<stacks; i++){
			float z = cos(M_PI*i/stacks);
			float mult = sin(M_PI*i/stacks);
			for (int j=0; j<slices; j++){
				float x = mult * cos(2*M_PI*j/slices);
				float y = mult * sin(2*M_PI*j/slices);
				sphereVerts[count++] = Vector3f(x, y, z);
			}
		}

		sphereVerts[count++] = Vector3f(0, 0, 1);
		sphereVerts[count++] = Vector3f(0, 0, -1);

		numSphereTris = slices * (stacks-2) * 2 + 2 * slices;
		sphereIndices =  (GLuint *) malloc(numSphereTris * 3 * sizeof(GLuint));
		count = 0;

		for (int i=1; i<stacks-1; i++){
			for (int j=0; j<slices; j++){
				int vertIndex = (i-1)*slices + j;
				int belowVertIndex = i*slices + j;
				if (j == slices-1){
					sphereIndices[3*count] = vertIndex;
					sphereIndices[3*count + 1] = belowVertIndex;
					sphereIndices[3*count + 2] = belowVertIndex - slices + 1;
					count++;
					sphereIndices[3*count] = vertIndex;
					sphereIndices[3*count + 1] = belowVertIndex - slices + 1;
					sphereIndices[3*count + 2] = vertIndex - slices + 1;
					count++;				
				} else {
					sphereIndices[3*count] = vertIndex;
					sphereIndices[3*count + 1] = belowVertIndex;
					sphereIndices[3*count + 2] = belowVertIndex + 1;
					count++;
					sphereIndices[3*count] = vertIndex;
					sphereIndices[3*count + 1] = belowVertIndex + 1;
					sphereIndices[3*count + 2] = vertIndex + 1;
					count++;
				}
			}
		}
		
		for (int j=0; j<slices; j++){
			int nPoleIndex = slices * (stacks-1);
			int sPoleIndex = nPoleIndex + 1;
			if (j == slices-1){
				int vertIndex = j;
				sphereIndices[3*count] = nPoleIndex;
				sphereIndices[3*count + 1] = vertIndex;
				sphereIndices[3*count + 2] = vertIndex - slices + 1;
				count++;
				vertIndex = slices * (stacks-2) + j;
				sphereIndices[3*count] = vertIndex;
				sphereIndices[3*count + 1] = sPoleIndex;
				sphereIndices[3*count + 2] = vertIndex - slices + 1;
				count++;
			} else {
				int vertIndex = j;
				sphereIndices[3*count] = nPoleIndex;
				sphereIndices[3*count + 1] = vertIndex;
				sphereIndices[3*count + 2] = vertIndex + 1;
				count++;
				vertIndex = slices * (stacks-2) + j;
				sphereIndices[3*count] = vertIndex;
				sphereIndices[3*count + 1] = sPoleIndex;
				sphereIndices[3*count + 2] = vertIndex + 1;
				count++;
			}
		}
		
	}

/* Generate unit cylinder with base at XY plane of radius 1 and height 1. You can use 10 as a good value for slices and stacks.*/
	void UnitCylinder(int slices, int stacks){
		if (cylinVerts != NULL) free(cylinVerts);
		if (cylinNormals != NULL) free(cylinNormals);
		if (cylinIndices != NULL) free(cylinIndices);
		numCylinVerts = slices * 2;
		cylinVerts =  (Vector3f *) malloc(numCylinVerts * sizeof(Vector3f));
		cylinNormals =  (Vector3f *) malloc(numCylinVerts * sizeof(Vector3f));
		int count = 0;
		for (int j=0; j<slices; j++){
			float x = 0.1*cos(2*M_PI*j/slices);
			float y = 0.1*sin(2*M_PI*j/slices);
			cylinNormals[count] = Vector3f(x,y,0);
			cylinVerts[count++] = Vector3f(x,y,0);
			cylinNormals[count] = Vector3f(x,y,0);
			cylinVerts[count++] = Vector3f(x,y,1);
		}

		numCylinTris = slices * 2;
		cylinIndices =  (GLuint *) malloc(numCylinTris*3 * sizeof(GLuint));
		count = 0;
		for (int j=0; j<slices-1; j++){
			int vertIndex = 2*j;
			int belowVertIndex = 2*j+1;
			int rightVertIndex = 2*j+2;
			int rightBelowVertIndex = 2*j+3;
			cylinIndices[3*count] = vertIndex;
			cylinIndices[3*count + 1] = belowVertIndex;
			cylinIndices[3*count + 2] = rightBelowVertIndex;
			count++;
			cylinIndices[3*count] = vertIndex;
			cylinIndices[3*count + 1] = rightBelowVertIndex;
			cylinIndices[3*count + 2] = rightVertIndex;
			count++;
		}
		cylinIndices[3*count] = 2*(slices-1);
		cylinIndices[3*count + 1] = 2*(slices-1) + 1;
		cylinIndices[3*count + 2] = 1;
		count++;
		cylinIndices[3*count] = 2*(slices-1);
		cylinIndices[3*count + 1] = 1;
		cylinIndices[3*count + 2] = 0;
		count++;
	}

};

#endif