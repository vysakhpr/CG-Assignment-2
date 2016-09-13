#ifndef BUFFER_H
#define BUFFER_H

#include "math_utils.h"
#include "protein.h"
#include "ligand.h"

ProteinMolecule protein; 
LigandMolecule ligand;

static void  CreateBuffers(char* OffFile)
{
	protein.LoadBuffers(OffFile);
	WorldBoundBox=protein.boundBox;
}

static void  CreateCRDBuffers(char* OffFile,char* CrdFile)
{
	ligand.LoadBuffers(CrdFile);
	protein.LoadBuffers(OffFile);
	WorldBoundBox=protein.boundBox;
}
#endif