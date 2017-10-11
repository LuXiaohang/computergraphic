#define _USE_MATH_DEFINES
#define GLM_FORCE_RADIANS

#include <cassert>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iterator>
#include <string>
#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <FreeImagePlus.h>
#include <GL/glut.h>


int Width = 1024, Height = 576;

using namespace glm;

GLuint program = 0;
GLuint coltex = 0;   //颜色纹理
GLuint normtex = 0;     // 法向纹理
double const TEXTURE_WIDTH = 20;  // 纹理宽度
GLuint waterMeshVAO = 0;
GLuint waterMeshBuffer = 0;
GLsizei waterMeshSize = 10;
GLuint flag1=0;
GLuint flag2=0;
GLuint flag3=0;
float angle1=1;

void checkGL()
{
#ifdef _DEBUG
	glFinish();
	GLenum error = glGetError();
	assert(error == GL_NO_ERROR);
#endif
}

GLuint loadShaderSource(const char *path, GLenum type)
{
	GLuint shader = glCreateShader(type);

	std::ifstream file(path);
	std::string sourceString((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	const char *sourcePointers[1] = {sourceString.c_str()};

	glShaderSource(shader, 1, sourcePointers, nullptr);
	glCompileShader(shader);

	char msg[1024] = "";
	glGetShaderInfoLog(shader, sizeof(msg), nullptr, msg);
	std::fprintf(stderr, "%s\n", msg);

	return shader;
}

void init()
{
	GLuint vShader = loadShaderSource("flame.vert", GL_VERTEX_SHADER);//顶点着色器
	GLuint fShader = loadShaderSource("flame.frag", GL_FRAGMENT_SHADER);//片元着色器

	program = glCreateProgram();
	glAttachShader(program, vShader);
	glAttachShader(program, fShader);
	glLinkProgram(program);

	char msg[1024] = "";
	glGetProgramInfoLog(program, sizeof(msg), nullptr, msg);
	std::fprintf(stderr, "%s\n", msg);

	glDeleteShader(vShader);
	glDeleteShader(fShader);

	fipImage image;

	image.load("pic1.gif");//加载图片1
	image.convertTo32Bits();

	glGenTextures(1, &coltex);
	glBindTexture(GL_TEXTURE_2D, coltex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, image.getWidth(), image.getHeight(), 0, GL_BGRA, GL_UNSIGNED_BYTE, image.accessPixels());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);

	image.load("pic2.gif");//加载图片2
	image.convertTo32Bits();

	glGenTextures(1, &normtex);
	glBindTexture(GL_TEXTURE_2D, normtex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, image.getWidth(), image.getHeight(), 0, GL_BGRA, GL_UNSIGNED_BYTE, image.accessPixels());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);

	float gridWidth = 10;
	GLfloat light_position[]={1.0,1.0,1.0,0.0};
	GLfloat ambient[]={0.5,0.0,0.0,1.0};
	GLfloat specular[]={1.0,1.0,1.0,1.0};
	GLfloat diffuse[]={0.5,0.5,0.5,1.0};
	glLightfv(GL_LIGHT0,GL_AMBIENT,ambient);
	glLightfv(GL_LIGHT0,GL_SPECULAR,specular);
	glLightfv(GL_LIGHT0,GL_POSITION,light_position);
	glLightfv(GL_LIGHT0,GL_DIFFUSE,diffuse);
	glEnable(GL_LIGHT0);  
    glEnable(GL_LIGHTING);   
    glEnable(GL_DEPTH_TEST); 
	glUseProgram(program);
	glUniform1i(glGetUniformLocation(program, "normtex"), 0);
	glUniform1i(glGetUniformLocation(program, "coltex"), 1);
	glUniform1f(glGetUniformLocation(program, "shiness"), 0.8);
	glUniform4f(glGetUniformLocation(program, "ambient"), 1,1,1,1.0);
	glUniform4f(glGetUniformLocation(program, "diffuse"), 0.5,0.5,0.5,1.0);
	glUniform4f(glGetUniformLocation(program, "specular"), 1.0,1.0,1.0,1.0);
	glUniform3f(glGetUniformLocation(program, "lightposition"), 10.0,10.0,100.0);
	glUniform4f(glGetUniformLocation(program, "eyeposition"), 0.0,0.0,10.0,0.0);
	glUseProgram(0);
	

	checkGL();
}

void drawground()
{
	glColor3f(1,1,1);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, normtex);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, coltex);

	glEnable(GL_TEXTURE_2D);
	glUseProgram(program);
	glBegin(GL_QUADS);

	glTexCoord2f(1, 1);
	glVertex2f(-5, 5);

	glTexCoord2f(1,0);
	glVertex2f(+5, +5);

	glTexCoord2f(0,0);
	glVertex2f(+5, -5);
	glTexCoord2f(0, 1);
	glVertex2f(-5, -5);
	glEnd();
	
	glUseProgram(0);

	//取消绑定
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, 0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void display()
{
	glClearColor(0, 0, 0, 0);
	glClearDepth(1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	float X=10*sin(angle1*M_PI/180);
	float Y=10*cos(angle1*M_PI/180);
	glEnable(GL_DEPTH_TEST);

	dmat4 viewMatrix = lookAt(dvec3(X, Y, 20), dvec3(0, 0, 0), dvec3(0, 0, 1));
	dmat4 projMatrix = perspective(M_PI / 4, double(Width) / Height, 0.5, 1000.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixd(value_ptr(viewMatrix));
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixd(value_ptr(projMatrix));
	glMatrixMode(GL_MODELVIEW);
	glColor3f(1.0,0,0);
	glutWireCube(10);
	drawground();

	glDisable(GL_DEPTH_TEST);

	checkGL();

	glutSwapBuffers();
	glutPostRedisplay();
}
void myboard(unsigned char key,int x,int y){
    switch (key) {
        case 'e':
         angle1++;
		 if(angle1>=360) 
			 angle1-=360;
            break;
		case 'r':
         angle1--;
		 if(angle1<=0) 
			 angle1+=360;
            break;
	}
    display();
}

void reshape(int width, int height)
{
	Width = width;
	Height = height;
	glViewport(0, 0, width, height);



	glBindTexture(GL_TEXTURE_2D, 0);
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_ALPHA | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(Width, Height);
	glutCreateWindow("OpenGL Window");

	glewInit();
	init();

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(myboard);
	glutMainLoop();

	return 0;
}
