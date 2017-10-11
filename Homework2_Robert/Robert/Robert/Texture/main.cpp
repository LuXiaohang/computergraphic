#define _USE_MATH_DEFINES

#include <cassert>
#include <cmath>
#include <cstdio>

#include <fstream>
#include <iterator>
#include <string>

#include <GL/glew.h>

#include <GL/GLU.h>
#include <GL/glut.h>

#include <FreeImagePlus.h>

//是否用shader进行绘制
int g_useShader = false;

const float cubeVertices[8][3] =
{
	{ -1, -1, -1 },
	{ +1, -1, -1 },
	{ -1, +1, -1 },
	{ +1, +1, -1 },
	{ -1, -1, +1 },
	{ +1, -1, +1 },
	{ -1, +1, +1 },
	{ +1, +1, +1 }
};

const unsigned cubeVertexIds[6][4] =
{
	{ 0, 1, 5, 4 },
	{ 1, 3, 7, 5 },
	{ 3, 2, 6, 7 },
	{ 2, 0, 4, 6 },
	{ 2, 3, 1, 0 },
	{ 4, 5, 7, 6 }
};

int windowWidth = 1024, windowHeight = 576;

GLuint program = 0;
GLint uniformCenter = -1;

GLuint textures[2] = {};

void checkGL()
{
	glFinish();
	GLenum error = glGetError();
	assert(error == GL_NO_ERROR);
}

void drawCube()
{
	glBegin(GL_QUADS);
	for (int side = 0; side < 6; ++side)
	{
		if (g_useShader)
		{
			glTexCoord2f(0, 0);
			glVertex3fv(cubeVertices[cubeVertexIds[side][0]]);
			glTexCoord2f(1, 0);
			glVertex3fv(cubeVertices[cubeVertexIds[side][1]]);
			glTexCoord2f(1, 1);
			glVertex3fv(cubeVertices[cubeVertexIds[side][2]]);
			glTexCoord2f(0, 1);
			glVertex3fv(cubeVertices[cubeVertexIds[side][3]]);
		}
		else
		{
			glMultiTexCoord2f(GL_TEXTURE0, 0.0f, 0.0f);
			glMultiTexCoord2f(GL_TEXTURE1, 0.0f, 0.0f);
			glVertex3fv(cubeVertices[cubeVertexIds[side][0]]);
			glMultiTexCoord2f(GL_TEXTURE0, 1.0f, 0.0f);
			glMultiTexCoord2f(GL_TEXTURE1, 1.0f, 0.0f);
			glVertex3fv(cubeVertices[cubeVertexIds[side][1]]);
			glMultiTexCoord2f(GL_TEXTURE0, 1.0f, 1.0f);
			glMultiTexCoord2f(GL_TEXTURE1, 1.0f, 1.0f);
			glVertex3fv(cubeVertices[cubeVertexIds[side][2]]);
			glMultiTexCoord2f(GL_TEXTURE0, 0.0f, 1.0f);
			glMultiTexCoord2f(GL_TEXTURE1, 0.0f, 1.0f);
			glVertex3fv(cubeVertices[cubeVertexIds[side][3]]);
		}
	}
	glEnd();
}

GLuint loadShaderSource(const char *path, GLenum type)
{
	GLuint shader = glCreateShader(type);

	std::ifstream file(path);
	std::string sourceString((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	const char *sourcePointers[1] = { sourceString.c_str() };

	glShaderSource(shader, 1, sourcePointers, nullptr);
	glCompileShader(shader);

	char msg[1024] = "";
	glGetShaderInfoLog(shader, sizeof(msg), nullptr, msg);
	std::fprintf(stderr, "%s\n", msg);

	return shader;
}

void init()
{
	if (g_useShader)
	{
		GLuint vShader = loadShaderSource("texture.vert", GL_VERTEX_SHADER);
		GLuint fShader = loadShaderSource("texture.frag", GL_FRAGMENT_SHADER);

		program = glCreateProgram();
		glAttachShader(program, vShader);
		glAttachShader(program, fShader);
		glLinkProgram(program);

		char msg[1024] = "";
		glGetProgramInfoLog(program, sizeof(msg), nullptr, msg);
		std::fprintf(stderr, "%s\n", msg);

		glDeleteShader(vShader);
		glDeleteShader(fShader);

		uniformCenter = glGetUniformLocation(program, "center");

		glUseProgram(program);
		glUniform1i(glGetUniformLocation(program, "tex0"), 0);
		glUniform1i(glGetUniformLocation(program, "tex1"), 1);
		glUseProgram(0);
	}

	glGenTextures(2, textures);

	fipImage image[2];
	image[0].load("texture0.png");
	image[1].load("texture1.png");

	image[0].convertTo32Bits();
	image[1].convertTo32Bits();

	glBindTexture(GL_TEXTURE_2D, textures[0]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, image[0].getWidth(), image[0].getHeight(), 0, GL_BGRA, GL_UNSIGNED_BYTE, image[0].accessPixels());
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textures[1]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, image[1].getWidth(), image[1].getHeight(), 0, GL_BGRA, GL_UNSIGNED_BYTE, image[1].accessPixels());
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void display()
{
	static double theta = 0;

	glClearColor(0, 0, 0, 0);
	glClearDepth(1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	double cameraX = std::cos(theta) * 20;
	double cameraY = std::sin(theta) * 20;
	theta += M_PI / 360.0;

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(cameraX, cameraY, 20, 0, 0, 0, 0, 0, 1);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, double(windowWidth) / double(windowHeight), 0.1, 1000.0);

	if (g_useShader)
		glUseProgram(program);
	else
	{
		glMatrixMode(GL_MODELVIEW);
	}
	glEnable(GL_DEPTH_TEST);

	glActiveTexture(GL_TEXTURE0);
	if (!g_useShader)
		glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textures[0]);
	if (!g_useShader)
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	glActiveTexture(GL_TEXTURE1);
	if (!g_useShader)
		glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textures[1]);
	if (!g_useShader)
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	for (int y = -20; y <= 20; y += 4)
	{
		for (int x = -20; x <= 20; x += 4)
		{
			if (g_useShader)
				glUniform3f(uniformCenter, float(x), float(y), 0.0f);
			else
			{
				glLoadIdentity();
				gluLookAt(cameraX, cameraY, 20, 0, 0, 0, 0, 0, 1);
				glTranslatef(float(x), float(y), 0.0f);
			}
			drawCube();
		}
	}

	glDisable(GL_DEPTH_TEST);
	if (g_useShader)
		glUseProgram(0);

	checkGL();

	glutSwapBuffers();
	glutPostRedisplay();
}

void reshape(int width, int height)
{
	windowWidth = width;
	windowHeight = height;
	glViewport(0, 0, width, height);
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_ALPHA | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(windowWidth, windowHeight);
	glutCreateWindow("OpenGL Window");

	glewInit();
	init();

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);

	glutMainLoop();

	return 0;
}
