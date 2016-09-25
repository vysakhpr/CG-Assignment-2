#ifndef BUFFER_H
#define BUFFER_H

#include "math_utils.h"
#include "protein.h"
#include "ligand.h"
#include "camera.h"
#include "trackball.h"
#include "lighting.h"

ProteinMolecule protein; 
LigandMolecule ligand;

Camera cam;
TrackBall track;
Lighting lights;


static void  CreateBuffers(char* OffFile)
{
	protein.LoadBuffers(OffFile);
	WorldBoundBox=protein.boundBox;
	cam.SetPosition(Vector3f(0,0,-WorldBoundBox.ZWidth));
}

static void  CreateCRDBuffers(char* OffFile,char* CrdFile)
{
	ligand.LoadBuffers(CrdFile);
	protein.LoadBuffers(OffFile);
	WorldBoundBox=BoundBox(protein.boundBox.Center,protein.boundBox.XWidth,protein.boundBox.YWidth,protein.boundBox.ZWidth,2);
	cam.SetPosition(Vector3f(0,0,-WorldBoundBox.ZWidth));
}
#endif