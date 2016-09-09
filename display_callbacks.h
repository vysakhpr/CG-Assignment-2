#ifndef DISPLAY_CALLBACKS_H
#define DISPLAY_CALLBACKS_H



#include "math_utils.h"
#include "shader.h"
#include "quaternion.h"
#include "camera.h"
#include "trackball.h"
#include "lighting.h"

Camera cam;
TrackBall track;
Lighting lights;


static void RenderScene()
{

	Matrix4f WorldProj, PersProj,CameraTrans, TrackballTrans, WorldTrans;
	TrackballTrans=track.RenderMatrix();
	CameraTrans=cam.RenderMatrix();
	PersProj.InitPersProjTransform(PersProjInfo(cam.FieldOfView(),WINDOW_WIDTH,WINDOW_HEIGHT,Scale*1,Scale*100));
	WorldTrans=TrackballTrans;
	WorldProj=PersProj*CameraTrans*WorldTrans;
	glUniformMatrix4fv(gWVPLocation,1,GL_TRUE,&WorldProj.m[0][0]);
	glUniformMatrix4fv(gWorldLocation,1,GL_TRUE,&WorldTrans.m[0][0]);

	DirectionalLight DLight=lights.GetDirectionalLight();
	glUniform3f(DLightColorLocation, DLight.Color.x, DLight.Color.y, DLight.Color.z);
    glUniform1f(DLightAmbientIntensityLocation, DLight.AmbientIntensity);
    Vector3f Direction=DLight.Direction;
    Direction.Normalize();
    glUniform3f(DLightDirectionLocation,Direction.x,Direction.y,Direction.z);
    glUniform1f(DLightDiffuseIntensityLocation,DLight.DiffuseIntensity);


	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,IBO);
	glDrawElements(GL_TRIANGLES,protein->numberOfTriangles*3,GL_UNSIGNED_INT,0);
	glBindVertexArray(0);
	GLenum errorCode = glGetError();
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);
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
	static int pre_x=WINDOW_WIDTH/2;
	static int pre_y=WINDOW_HEIGHT/2;
	int dx=x - pre_x;
	int dy=y- pre_y;
	pre_x=x;
	pre_y=y;
	track.SetAngles(-dx,-dy,x,y);
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
		case 'L': 	cam.ToggleLock();	break;
		case 'r':
		case 'R': 	cam.ResetCamera();track.ResetTrackBall();break;
		case 'a':
		case 'A':	lights.SwitchDirectionalLightOn();break;
		case 'z':
		case 'Z':	lights.SwitchDirectionalLightOff();break;
		default: exit(1);
	}
}

#endif