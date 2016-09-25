#ifndef DISPLAY_CALLBACKS_H
#define DISPLAY_CALLBACKS_H


#include "math_utils.h"
#include "shader.h"
#include "quaternion.h"
#include "buffer.h"

bool ChainColorFlag=false;
bool ExplodeFlag=false;

struct MouseMotion
{
	int pre_x;
	int pre_y;
}mouse;
static void RenderScene()
{
	glUniform1i(ChainColorFlagLocation,ChainColorFlag);
	glUniform1i(ExplodeFlagLocation,ExplodeFlag);
	lights.SetPositionalLightPosition(Vector3f(protein.boundBox.XWidth/2,protein.boundBox.YWidth/2,-protein.boundBox.ZWidth/2));

	Matrix4f WorldProj, PersProj,CameraTrans, TrackballTrans, WorldTrans;
	TrackballTrans=track.RenderMatrix();
	CameraTrans=cam.RenderMatrix();
	PersProj.InitPersProjTransform(PersProjInfo(cam.FieldOfView(),WINDOW_WIDTH,WINDOW_HEIGHT,0.1,10000));
	WorldTrans=TrackballTrans;
	WorldProj=PersProj*CameraTrans*WorldTrans;            

	glUniformMatrix4fv(gWVPLocation,1,GL_TRUE,&WorldProj.m[0][0]);
	glUniformMatrix4fv(gWorldLocation,1,GL_TRUE,&WorldTrans.m[0][0]);
	glUniform1i(LigandFlagLocation,0);

	SetLightsInShader(lights,cam);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	protein.RenderDisplay();
	if(CRD_FILE)
	{	
		ligand.RenderDisplay(cam);	
	}
	

	GLenum errorCode = glGetError();
	if (errorCode == GL_NO_ERROR) {
		glutSwapBuffers();
	} else {
		fprintf(stderr, "OpenGL rendering error %d\n", errorCode);
	}
}

static void onIdle()
{
	glutPostRedisplay();
}


static void onPassiveMouseMotion(int x, int y)
{
	static int pre_mouse_x=WINDOW_WIDTH/2;
	static int pre_mouse_y=WINDOW_HEIGHT/2;
	int dx=x - pre_mouse_x;
	int dy=y- pre_mouse_y;
	pre_mouse_x=x;
	pre_mouse_y=y;
	cam.SetAngles(dx,dy,x,y);
	glutPostRedisplay();
}

static void onActiveMouseMotion(int x, int y)
{
	int dx=x - mouse.pre_x;
	int dy=y- mouse.pre_y;
	mouse.pre_x=x;
	mouse.pre_y=y;
	track.SetAngles(-dx,-dy,x,y);
	glutPostRedisplay();
}


static void onMousePress(int button, int state, int x, int y)
{
	if(state==GLUT_DOWN)
	{
		mouse.pre_x=x;
		mouse.pre_y=y;
	}

	if(button==3 && cam.FieldOfView() != 1)
	{
		cam.ZoomIn();
	}
	else if(button==4 && cam.FieldOfView() != 179)
	{
		cam.ZoomOut();
	}
	glutPostRedisplay();
}

static void onReshape(int width, int height) 
{
	glViewport(0, 0, width, height);	
	WINDOW_WIDTH = width;
	WINDOW_HEIGHT = height;
}

static void onKeyPress(unsigned char key, int x, int y)
{
	switch(key)
	{
		case 'l':
		case 'L': 	cam.ToggleLock();	break;
		case 'r':
		case 'R': 	cam.ResetCamera();track.ResetTrackBall();break;
		case 'a':
		case 'A':	lights.ToggleDirectionalLightSwitch();break;
		case 'z':
		case 'Z':	lights.TogglePositionalLightSwitch();break;

		case 'c':
		case 'C':	ChainColorFlag=!ChainColorFlag;break;
		case 'v':
		case 'V': 	protein.ToggleColoring();
		case 'e':
		case 'E': 	ExplodeFlag=!ExplodeFlag;break;
		case 'x':
		case 'X':	protein.ToggleChainEdge();break;
		case 's':
		case 'S':	protein.ToggleAminoEdge();break;
		default:glUniform1i(LigandFlagLocation,0); 
				exit(1);
	}
}

#endif