#include <GL/glew.h>
#include <GL/freeglut.h>
#include "file_utils.h"
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <new>

int WINDOW_HEIGHT=1000;
int WINDOW_WIDTH=1000;

GLuint VBO,VAO,IBO;
GLuint gWorldLocation;
float Scale;
const char* pVSFileName = "shader.vs";
const char* pFSFileName = "shader.fs";
const char* pGSFileName = "shader.gs";


#include "helper.h"
#include "buffer.h"
#include "display_callbacks.h"



static void AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType) {
	GLuint ShaderObj = glCreateShader(ShaderType);

	if (ShaderObj == 0) {
		fprintf(stderr, "Error creating shader type %d\n", ShaderType);
		exit(0);
	}

	const GLchar * p[1];
	p[0] = pShaderText;
	GLint Lengths[1];
	Lengths[0] = strlen(pShaderText);
	glShaderSource(ShaderObj, 1, p, Lengths);
	glCompileShader(ShaderObj);
	GLint success;
	glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);
	if (!success) {
		GLchar InfoLog[1024];
		glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
		fprintf(stderr, "Error compiling shader type %d: '%s'\n", ShaderType, InfoLog);
		exit(1);
	}

	glAttachShader(ShaderProgram, ShaderObj);
}

using namespace std;

static void CompileShaders() {
	GLuint ShaderProgram = glCreateProgram();

	if (ShaderProgram == 0) {
		fprintf(stderr, "Error creating shader program\n");
		exit(1);
	}

	string vs, fs, gs;

	if (!ReadFile(pVSFileName, vs)) {
		exit(1);
	}

	if (!ReadFile(pFSFileName, fs)) {
		exit(1);
	}

	if (!ReadFile(pGSFileName, gs)) {
		exit(1);
	}

	AddShader(ShaderProgram, vs.c_str(), GL_VERTEX_SHADER);
	AddShader(ShaderProgram, fs.c_str(), GL_FRAGMENT_SHADER);
	//AddShader(ShaderProgram, gs.c_str(), GL_GEOMETRY_SHADER);

	GLint Success = 0;
	GLchar ErrorLog[1024] = {0};

	glLinkProgram(ShaderProgram);
	glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &Success);
	if (Success == 0) {
		glGetProgramInfoLog(ShaderProgram, sizeof (ErrorLog), NULL, ErrorLog);
		fprintf(stderr, "Error linking shader program: '%s'\n", ErrorLog);
		exit(1);
	}

	glValidateProgram(ShaderProgram);
	glGetProgramiv(ShaderProgram, GL_VALIDATE_STATUS, &Success);
	if (!Success) {
		glGetProgramInfoLog(ShaderProgram, sizeof (ErrorLog), NULL, ErrorLog);
		fprintf(stderr, "Invalid shader program: '%s'\n", ErrorLog);
		exit(1);
	}

	glUseProgram(ShaderProgram);
	gWorldLocation=glGetUniformLocation(ShaderProgram,"gWorld");

}

static void InitializeDisplayCallbacks()
{
	glutDisplayFunc(RenderScene);
	glutIdleFunc(onIdle);
	glutMouseFunc(onMousePress);
	glutPassiveMotionFunc(onMouseMotion);
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