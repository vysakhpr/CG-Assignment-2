#include <GL/glew.h>
#include <GL/freeglut.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <new>

int WINDOW_HEIGHT=1000;
int WINDOW_WIDTH=1000;

bool OFF_FILE = false;
bool CRD_FILE = false;



const char* pVSFileName = "shader.vs";
const char* pFSFileName = "shader.fs";
const char* pGSFileName = "shader.gs";



#include "include/file_utils.h"
#include "include/math_utils.h"
struct BoundBox
{
	Vector3f Center;
	float XWidth;
	float YWidth;
	float ZWidth;

	BoundBox()
	{
		
	}
	BoundBox(Vector3f v,float x, float y, float z,int i)
	{
		Center=v;
		XWidth=i*x;
		YWidth=i*y;
		ZWidth=i*z;
	}
}WorldBoundBox;


#include "include/shader.h"
#include "include/lighting.h"
#include "include/helper.h"

#include "include/buffer.h"
#include "include/display_callbacks.h"


static void InitializeDisplayCallbacks()
{
	glutDisplayFunc(RenderScene);
	glutIdleFunc(onIdle);
	glutMouseFunc(onMousePress);
	glutPassiveMotionFunc(onPassiveMouseMotion);
	glutMotionFunc(onActiveMouseMotion);
	glutReshapeFunc(onReshape);
	glutKeyboardFunc(onKeyPress);
}

static void onInit(int argc, char ** argv)
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	if(argc==2)
	{
		OFF_FILE=true;
	}
	if(argc==3)
	{
		CRD_FILE=true;
	}
	if(argc<2 || argc >3)
	{
		cout<<"ENTER THE  FILES"<<endl;
		exit(1);
	}

	if(CRD_FILE)
		CreateCRDBuffers(argv[1],argv[2]);
	else	
		CreateBuffers(argv[1]);

	CompileShaders();

	glEnable(GL_DEPTH_TEST); 

}

int main(int argc, char**argv)
{	
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGBA);
	glutInitWindowSize(WINDOW_WIDTH,WINDOW_HEIGHT);
	glutInitWindowPosition(100,100);
	glutCreateWindow("Protein Viewer");
	InitializeDisplayCallbacks();
	GLenum res = glewInit();
	if (res != GLEW_OK) {
		fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
		return 1;
	}
	onInit(argc,argv);
	glutWarpPointer(WINDOW_WIDTH/2,WINDOW_HEIGHT/2);
	glClearColor(0.0f,0.0f,0.0f,0.0f);
	glutMainLoop();
	return(0);
}