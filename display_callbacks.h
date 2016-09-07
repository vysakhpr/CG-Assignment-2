float ZoomFactor=45.0f;

static void RenderScene()
{
	Matrix4f WorldProj, PersProj, CameraTranslateTrans,CameraTrans;
	CameraTranslateTrans.InitTranslationTransform(0,0,boundBox.ZWidth);
	CameraTrans.InitCameraTransform(Vector3f(0,0,1),Vector3f(0,1,0));
	PersProj.InitPersProjTransform(PersProjInfo(90-ZoomFactor,WINDOW_WIDTH,WINDOW_HEIGHT,Scale*1,Scale*100));
	WorldProj=PersProj*CameraTrans*CameraTranslateTrans;

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


static void onReshape(int width, int height) 
{
	glViewport(0, 0, width, height);	
	WINDOW_WIDTH = width;
	WINDOW_HEIGHT = height;
}

static void onMousePress(int button, int state, int x, int y)
{
	if(button==3 && ZoomFactor != 89)
	{
		ZoomFactor+=1;
	}
	else if(button==4 && ZoomFactor != -89)
	{
		ZoomFactor-=1;
	}
	glutPostRedisplay();
}