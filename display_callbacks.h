#ifndef DISPLAY_CALLBACKS_H
#define DISPLAY_CALLBACKS_H



#include "math_utils.h"
#include "quaternion.h"
#include "camera.h"

Camera cam;



static void RenderScene()
{

	Matrix4f WorldProj, PersProj,CameraTrans;

	CameraTrans=cam.RenderMatrix();

	PersProj.InitPersProjTransform(PersProjInfo(cam.FieldOfView(),WINDOW_WIDTH,WINDOW_HEIGHT,Scale*1,Scale*100));

	WorldProj=PersProj*CameraTrans;

	glUniformMatrix4fv(gWorldLocation,1,GL_TRUE,&WorldProj.m[0][0]);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,IBO);
	glDrawElements(GL_TRIANGLES,protein->numberOfTriangles*3,GL_UNSIGNED_INT,0);
	glBindVertexArray(0);
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


static void onMouseMotion(int x, int y)
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

static void onMousePress(int button, int state, int x, int y)
{
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
		case 'L': cam.ToggleLock();	break;
		case 'r':
		case 'R': cam.ResetCamera();break;
	}
}

#endif