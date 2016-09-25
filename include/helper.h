#ifndef HELPER_H
#define HELPER_H



#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <GL/glew.h>
#include "math_utils.h"

int ret;

/* Atomic radii for the different atom types */
const float atomic_radii[]  = {1.100, 1.348, 2.000, 1.400, 1.548, 1.808, 1.900, 1.26};

/* Recommended colors for the different atom types */
unsigned char atomColors[][3] = {
        {0x80, 0x80, 0x80}, //Dark Gray
        {0x00, 0x00, 0xFF}, //Blue
        {0xFF, 0x00, 0x00}, //Red
        {0xFF, 0xFF, 0xFF}, //White
        {0xFF, 0xFF, 0x00}, //Yellow
        {0xFF, 0x7F, 0xAA}, //Orange
        {0x00, 0x80, 0x00}, //Green
        {0x88, 0x00, 0x00}, //Dark Red
};

/* Recommended colors for amino acid types, same colors can be used for chainID as well */
unsigned char aminoColors[][3] = {
        {0x00, 0x7D, 0x34}, //Vivid Green
        {0x80, 0x3E, 0x75}, //Strong Purple
        {0xFF, 0x68, 0x00}, //Vivid Orange
        {0xC1, 0x00, 0x20}, //Vivid Red
        {0xCE, 0xA2, 0x62}, //Grayish Yellow
        {0x81, 0x70, 0x66}, //Medium Gray
        {0xFF, 0xB3, 0x00}, //Vivid Yellow
        {0xF6, 0x76, 0x8E}, //Strong Purplish Pink
        {0x00, 0x53, 0x8A}, //Strong Blue
        {0xFF, 0x7A, 0x5C}, //Strong Yellowish Pink
        {0x53, 0x37, 0x7A}, //Strong Violet
        {0xFF, 0x8E, 0x00}, //Vivid Orange Yellow
        {0xA6, 0xBD, 0xD7}, //Very Light Blue
        {0xB3, 0x28, 0x51}, //Strong Purplish Red
        {0x7F, 0x18, 0x0D}, //Strong Reddish Brown
        {0xF4, 0xC8, 0x00}, //Vivid Greenish Yellow
        {0x93, 0xAA, 0x00}, //Vivid Yellowish Green
        {0x59, 0x33, 0x15}, //Deep Yellowish Brown
        {0xF1, 0x3A, 0x13}, //Vivid Reddish Orange
        {0x23, 0x2C, 0x16}, //Dark Olive Green

        {0x00, 0x00, 0x00}, //Black
};

/* Handling CRD file -- Start */
typedef struct At {
	float x,y,z;
	int type;
}Atom;

typedef struct Bd {
	int atom1, atom2;
}Bond;

typedef struct molecule {
	Atom *atoms;
	Bond *bonds;
	int numAtoms;
 	int numBonds;
 	float srcCoords[3];
 	float destCoords[3];
}Molecule;

Molecule* readCrdFile(char * crdFile) {
	FILE * input;
	int i;
	int nAtoms, nBonds;
	float x,y,z;
	int type, atm1, atm2;
	Molecule *mol;

	input = fopen(crdFile, "r");
	
	mol = (Molecule*) malloc(sizeof(Molecule));
	
	/* Read the source and destination coordinates of the ligand */
	ret=fscanf(input, "%f %f %f", &x,&y,&z);
	mol->srcCoords[0] = x;
	mol->srcCoords[1] = y;
	mol->srcCoords[2] = z;
	ret=fscanf(input, "%f %f %f", &x,&y,&z);
	mol->destCoords[0] = x;
	mol->destCoords[1] = y;
	mol->destCoords[2] = z;
	
	/* Read the no. of atoms and bonds */
	ret=fscanf(input, "%d", &nAtoms);
	ret=fscanf(input, "%d", &nBonds);

	mol->numAtoms = nAtoms;
	mol->numBonds = nBonds;
	
	/* allocate required data */
	mol->atoms = (Atom *) malloc(nAtoms * sizeof(Atom));
	mol->bonds = (Bond *) malloc(nBonds * sizeof(Bond));
	
	/* Read the Atoms*/	
	for(i = 0; i < nAtoms; i++) {
		ret=fscanf(input, "%f %f %f %d", &x,&y,&z,&type);
		(mol->atoms[i]).x = x;
		(mol->atoms[i]).y = y;
		(mol->atoms[i]).z = z;
		(mol->atoms[i]).type = type;
	}

	/* Read the Bonds */	
	for(i = 0;i < nBonds; i++) {
		ret=fscanf(input, "%d %d", &atm1, &atm2);
		(mol->bonds[i]).atom1 = atm1;
		(mol->bonds[i]).atom2 = atm2;
	}

	fclose(input);
	return mol;
}

int FreeMolecule(Molecule *mol)
{
	//int i,j;
	if( mol == NULL )
		return 0;
	free(mol->atoms);
	free(mol->bonds);
	free(mol);
	return 1;
}
/* Handling CRD file -- End */

/* Handling OFF file -- Start */
typedef struct Vt {
	float x,y,z;
	Vector3f Normal;
}Vertex;

typedef struct Tri {
	int v[3];
	int aminoID;
	int aminoType;
	int chainID;	
}Triangle;

typedef struct offmodel {
	Vertex *vertices;
	Triangle *triangles;
	int numberOfVertices;
 	int numberOfTriangles;
}OffModel;

OffModel* readOffFile(char * OffFile) {
	FILE * input;
	char type[3]; 
	int noEdges;
	int i,j;
	float x,y,z;
	//int n, v;
	int v;
	int nv, np;
	int aminoID, aminoType, chainID;	
	OffModel *model;
	input = fopen(OffFile, "r");
	ret=fscanf(input, "%s", type);
	/* First line should be OFF */
	if(strcmp(type,"OFF")) {
		printf("Not a OFF file");
		exit(1);
	}
	/* Read the no. of vertices, faces and edges */
	ret=fscanf(input, "%d", &nv);
	ret=fscanf(input, "%d", &np);
	ret=fscanf(input, "%d", &noEdges);

	model = (OffModel*)malloc(sizeof(OffModel));
	model->numberOfVertices = nv;
	model->numberOfTriangles = np;
	
	
	/* allocate required data */
	model->vertices = (Vertex *) malloc(nv * sizeof(Vertex));
	model->triangles = (Triangle *) malloc(np * sizeof(Triangle));
	

	/* Read the vertices' location*/	
	for(i = 0;i < nv;i++) {
		ret=fscanf(input, "%f %f %f", &x,&y,&z);
		(model->vertices[i]).x = x;
		(model->vertices[i]).y = y;
		(model->vertices[i]).z = z;
		(model->vertices[i]).Normal=Vector3f(0.0,0.0,0.0);

	}

	/* Read the Triangles */	
	for(i = 0;i < np;i ++) {
		/* read the vertices */
		for(j = 0;j < 3;j ++) {
			ret=fscanf(input, "%d", &v);
			(model->triangles[i]).v[j] = v;
		}
		ret=fscanf(input, "%d %d %d", &aminoID, &aminoType, &chainID);
		(model->triangles[i]).aminoID = aminoID;
		(model->triangles[i]).aminoType = aminoType;
		(model->triangles[i]).chainID = chainID;
	}

	fclose(input);
	return model;
}

int FreeOffModel(OffModel *model)
{
	if( model == NULL )
		return 0;
	free(model->vertices);
	free(model->triangles);
	free(model);
	return 1;
}
/* Handling OFF file -- End */

/* Generate unit sphere and unit cylinder -- start */
/*
Vector3f *sphereVerts = NULL;
GLuint *sphereIndices = NULL;
int numSphereVerts = 0;
int numSphereTris = 0;

Vector3f *cylinVerts = NULL;
Vector3f *cylinNormals = NULL;
GLuint *cylinIndices = NULL;
int numCylinVerts = 0;
int numCylinTris = 0;

GLuint SPH_VBO, CYL_VBO, CYL_NORM_VBO, SPH_IBO, CYL_IBO;
*/
/* Generate unit sphere centered at origin of radius 1. The parameters slices and stacks 
control the number of triangles generated for the sphere. You can use 20 as a good value for slices and stacks. */
/*void UnitSphere(int slices, int stacks){
	if (sphereVerts != NULL) free(sphereVerts);
	if (sphereIndices != NULL) free(sphereIndices);
	numSphereVerts = slices * (stacks-1) + 2;
	sphereVerts =  (Vector3f *) malloc(numSphereVerts * sizeof(Vector3f));
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
	sphereIndices =  (GLuint *) malloc(numSphereTris * sizeof(GLuint *));
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
*/
/* Generate unit cylinder with base at XY plane of radius 1 and height 1. You can use 10 as a good value for slices and stacks.*/
/*void UnitCylinder(int slices, int stacks){
	if (cylinVerts != NULL) free(cylinVerts);
	if (cylinNormals != NULL) free(cylinNormals);
	if (cylinIndices != NULL) free(cylinIndices);
	numCylinVerts = slices * 2;
	cylinVerts =  (Vector3f *) malloc(numCylinVerts * sizeof(Vector3f));
	cylinNormals =  (Vector3f *) malloc(numCylinVerts * sizeof(Vector3f));
	int count = 0;
	for (int j=0; j<slices; j++){
		float x = cos(2*M_PI*j/slices);
		float y = sin(2*M_PI*j/slices);
		cylinNormals[count] = Vector3f(x, y, 0);
		cylinVerts[count++] = Vector3f(x, y, 0);
		cylinNormals[count] = Vector3f(x, y, 0);
		cylinVerts[count++] = Vector3f(x, y, 1);
	}

	numCylinTris = slices * 2;
	cylinIndices =  (GLuint *) malloc(numCylinTris * sizeof(Vector3f));
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
*/
/* Generate unit sphere and unit cylinder -- end */


#endif