This folder contains following files:

--------------------------------------------------------------------------------
Protein mesh files: 
1. 1duk.off
2. 4hhb.off
3. 1grm.off
4. 2oar.off
5. 3sy7.off

As you can notice, three of these are same proteins are same as you used in 
previous assignment. You can use these three proteins for all the tasks except
for task 3. For task 3, you need a protein with ligands. These are the two new 
protein meshes viz. 1duk and 4hhb. 

There is minor difference between the format of these mesh files and
the meshes provided for Assignment-1. Here each triangle has three extra 
fields viz. amino_ID, amino_type, and chain_ID. You will have to use these 
new fields for completing the task 4.
--------------------------------------------------------------------------------

--------------------------------------------------------------------------------
Ligand files:
1. 1duk_lig_HEM_A.crd
2. 4hhb_lig_HEM_A.crd
3. 4hhb_lig_HEM_B.crd
4. 4hhb_lig_HEM_C.crd
5. 4hhb_lig_HEM_D.crd

Note that ligand files are provided for only two protein meshes. Also, one 
protein mesh has a single ligand while the other has four ligands. You can match
the protein mesh with its corresponding ligand(s) based on first four characters 
of the file names.

CRD is a simple file format:  
* first line : Source coordinate of the ligand.
* second line : Destination coordinate.
* next line : #number_of_atoms #number_of_bonds
* next #number_of_atoms lines have atom info : x y z coordinates of the atom's 
position, followed by the type of atom (an integer between 1 and 8)
* next #number_of_bonds lines have bond info : atom1 atom2 (the indices of 
atoms which this bond connects).

The radius of the atom depends on the atom type. Use following atomic radii 
(these values are available in helper.c file):

--------------------------------------------------------------------------------
  	   Atom_type          Radius                  Element
--------------------------------------------------------------------------------
            1               1.100                  Hydrogen
            2               1.328                   Oxygen
            3               2.000                 Chlorine 
            4               1.400                  Nitrogen
            5               1.548                   Carbon
            6               1.808                   Sulphur
            7               1.900                 Phosphorus
            8               1.260                    Iron

For ball-and-stick representation, which you have to implement for task 3(a), 
I recommend that you use a scaling factor of 0.2 to scale down the given radii. 
For bonds, you can use cylinders of radius 0.2. However, I would encourage you 
to experiment by changing these values, and settle with what you feel is the 
best.

Recommended colors for various atoms are provided in helper.c. Use these colors
for rendering atoms in the ligands. You can use white/gray color for coloring 
the tubes representing the bonds.
--------------------------------------------------------------------------------

--------------------------------------------------------------------------------
ligand_data.txt

This files provides the initial and final positions of the center of the 
bounding box of the provided ligand molecules. You will have to use these 
coordinates for Tasks 3(b) and 3(c).
--------------------------------------------------------------------------------

--------------------------------------------------------------------------------
helper.h

Header file containing helper methods like readers for OFF and CRD files and for 
generating geometry of a unit sphere and cylinder. It also contains recommended
colors for coloring atoms in the ligand, and coloring patches in Task 4. 

Note that sphere and cylinder generation code uses global variables for storing 
the vertices, normals and VBO ids, etc. So, it may be better if you copy that
part of the code to your main file. 

Please note that I have not tested this code thoroughly. Please let me know if 
you face any problems.
--------------------------------------------------------------------------------


