#include <GL/glew.h>
#include <GL/freeglut.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <new>

int WINDOW_HEIGHT=1000;
int WINDOW_WIDTH=1000;


float Scale;
const char* pVSFileName = "shader.vs";
const char* pFSFileName = "shader.fs";
const char* pGSFileName = "shader.gs";

#include "file_utils.h"
#include "shader.h"
#include "lighting.h"
#include "helper.h"

#include "buffer.h"
#include "display_callbacks.h"



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
	if(argc!=2)
	{
		cout<<"ENTER THE OFF FILE"<<endl;
		exit(1);
	}	
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