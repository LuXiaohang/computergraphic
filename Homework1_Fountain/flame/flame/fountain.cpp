#define _USE_MATH_DEFINES

#include <cassert>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <time.h>
#include <fstream>
#include <iterator>
#include <string>

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <FreeImagePlus.h>

#include <GL/glut.h>

int windowWidth = 1024, windowHeight = 576;

GLuint program = 0;
GLuint texture = 0;
GLuint ground[6];

using namespace glm;

struct FlameParticle
{
	double life;            // 剩余寿命
	double fade;            // 衰减速率
	double size;            // 大小
	double angle;           // 旋转
	dvec3 pos;              // 位置
	dvec3 vel;              // 速度
	vec4 colorFirst;        // 初始颜色
	vec4 colorEnd;          // 末端颜色
};

int const NUM_PARTICLES = 25000;

FlameParticle particles[NUM_PARTICLES];

dvec3 const INIT_POS(0, 1.5, -2);
dvec3 const INIT_VEL = normalize(dvec3(0, 0, 1)) * 17.5;
double const INIT_SIZE = 0.25;
double const SIZE_SCALING = 0.25;
double const ROT_SPEED = M_PI / 0.1;

double randomFade()
{
	double duration = double(rand()) / RAND_MAX + 1;
	return 1.0 / duration;
}

double randDouble()
{
	return rand() / double(RAND_MAX);
}
//产生01之间的数字
void initParticle(FlameParticle &p)
{
	p.life = 1.0;
	p.fade = randomFade();
	p.size = INIT_SIZE;
	p.angle = randDouble() * 5.0 * M_PI;
	p.pos = INIT_POS;
	p.vel = INIT_VEL;


	double a=(randDouble()-0.5);

	double b=(randDouble()-0.5);

	dvec3 v=normalize(dvec3(a,b,0))*4.0;
	p.vel+=v;
	p.colorFirst = vec4(0.5, 0.5,0.6,1.0);
	p.colorEnd = vec4(0.5, 0.5, 0.6, 1.0);
}

dvec3 randomOffset()//产生一个随机的向量
{
	return vec3(randDouble(), randDouble(), randDouble());
}
/*
cnst float cubeVertices[8][3] =
{
	{ -5, -3, -3 },
	{ +3, -3, -3 },
	{ -5, +3, -3 },
	{ +3, +3, -3 },
	{ -5, -3, -2 },
	{ +3, -3, -2 },
	{ -5, +3, -2 },
	{ +3, +3, -2 }
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
*/
 
void drawGround()
{
	/*
	glFrontFace(GL_CCW); 
    glCullFace(GL_BACK); 
    glEnable(GL_CULL_FACE); 
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	*/
	
	glGenTextures(6, ground);

	fipImage image[6];
	image[0].load("rock.bmp");
	image[1].load("water.bmp");
	image[2].load("rock.bmp");
	image[3].load("rock.bmp");
	image[4].load("rock.bmp");
	image[5].load("rock.bmp");

	for(int i=0;i<6;i++){
		image[i].convertTo32Bits();
		glBindTexture(GL_TEXTURE_2D, ground[i]);
	    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, image[i].getWidth(), image[i].getHeight(), 0, GL_BGRA, GL_UNSIGNED_BYTE, image[i].accessPixels());
	    glGenerateMipmap(GL_TEXTURE_2D);
	}
	glBindTexture(GL_TEXTURE_2D, 0);

	glEnable(GL_TEXTURE_2D);
	/*
	glBindTexture(GL_TEXTURE_2D, ground[0]);
    glBegin(GL_QUADS); 
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-5.0f, -3.0f,  -2.0f); 
    glTexCoord2f(1.0f, 0.0f); glVertex3f(3.0f, -3.0f,  -2.0f); 
    glTexCoord2f(1.0f, 1.0f); glVertex3f(3.0f, 3.0f,  -2.0f); 
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-5.0f, 3.0f,  -2.0f); 
    glEnd(); 
	*/
 

	glBindTexture(GL_TEXTURE_2D, ground[1]);
    glBegin(GL_QUADS); 
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-5.0f, -3.0f,  -3.0f); 
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-5.0f, 3.0f,  -3.0f); 
    glTexCoord2f(0.0f, 1.0f); glVertex3f(3.0f, 3.0f,  -3.0f); 
    glTexCoord2f(0.0f, 0.0f); glVertex3f(3.0f, -3.0f,  -3.0f); 
    glEnd(); 
 
  
	glBindTexture(GL_TEXTURE_2D, ground[2]);
    glBegin(GL_QUADS); 
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-5.0f, 3.0f,  -3.0f); 
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-5.0f, 3.0f,  -2.0f); 
    glTexCoord2f(1.0f, 0.0f); glVertex3f(3.0f, 3.0f,  -2.0f); 
    glTexCoord2f(1.0f, 1.0f); glVertex3f(3.0f, 3.0f,  -3.0f); 
    glEnd(); 
 

	glBindTexture(GL_TEXTURE_2D, ground[3]);
    glBegin(GL_QUADS); 
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-5.0f, -3.0f,  -3.0f); 
    glTexCoord2f(0.0f, 1.0f); glVertex3f(3.0f, -3.0f,  -3.0f); 
    glTexCoord2f(0.0f, 0.0f); glVertex3f(3.0f, -3.0f,  -2.0f); 
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-5.0f, -3.0f,  -2.0f); 
    glEnd(); 
 

	glBindTexture(GL_TEXTURE_2D, ground[4]);
    glBegin(GL_QUADS); 
    glTexCoord2f(1.0f, 0.0f); glVertex3f(3.0f, -3.0f,  -3.0f); 
    glTexCoord2f(1.0f, 1.0f); glVertex3f(3.0f, 3.0f,  -3.0f); 
    glTexCoord2f(0.0f, 1.0f); glVertex3f(3.0f, 3.0f,  -2.0f); 
    glTexCoord2f(0.0f, 0.0f); glVertex3f(3.0f, -3.0f,  -2.0f); 
    glEnd(); 
 
 
	glBindTexture(GL_TEXTURE_2D, ground[5]);
    glBegin(GL_QUADS); 
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-5.0f, -3.0f,  -3.0f); 
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-5.0f, -3.0f,  -2.0f); 
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-5.0f, 3.0f,  -2.0f); 
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-5.0f, 3.0f,  -3.0f); 
    glEnd(); 


}

void step(FlameParticle &p, double delta_t)
{
	p.life -= p.fade * delta_t;
	if(p.life <= 0.0)
		initParticle(p);
	else
	{
		dvec3 g=dvec3(0,0,-0.6);
		p.vel+=g;
		p.pos += p.vel * delta_t;
		p.angle += 5*ROT_SPEED * randDouble() * delta_t;
		p.size -= SIZE_SCALING * delta_t;
	}
}

void checkGL()
{
	glFinish();
	GLenum error = glGetError();
	assert(error == GL_NO_ERROR);
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
	GLuint vShader = loadShaderSource("flame.vert", GL_VERTEX_SHADER);
	GLuint gShader = loadShaderSource("flame.geom", GL_GEOMETRY_SHADER);
	GLuint fShader = loadShaderSource("flame.frag", GL_FRAGMENT_SHADER);

	program = glCreateProgram();
	glAttachShader(program, vShader);
	glAttachShader(program, gShader);
	glAttachShader(program, fShader);
	glBindAttribLocation(program, 0, "p_pos");
	glBindAttribLocation(program, 1, "p_size");
	glBindAttribLocation(program, 2, "p_color");
	glBindAttribLocation(program, 3, "p_rotation");
	glLinkProgram(program);

	char msg[1024] = "";
	glGetProgramInfoLog(program, sizeof(msg), nullptr, msg);
	std::fprintf(stderr, "%s\n", msg);

	glDeleteShader(vShader);
	glDeleteShader(gShader);
	glDeleteShader(fShader);

	fipImage image;
	image.load("particle.png");
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, image.getWidth(), image.getHeight(),
		0, GL_RED, GL_UNSIGNED_BYTE, image.accessPixels());
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);

	for(int i = 0; i < NUM_PARTICLES; ++i)
		initParticle(particles[i]);
	
	// 预先迭代 100 帧，达到比较均匀的分布，仅作为演示使用
	for(int i = 0; i < 500; ++i)
	{
		for(int j = 0; j < NUM_PARTICLES; ++j)
			step(particles[j], 1.0 / 60.0);
	}

	checkGL();
}

void display()
{
	glClearColor(0, 0, 0, 0);
	glClearDepth(1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	float X=10*sin(30*M_PI/180);
	float Y=10*cos(30*M_PI/180);


	dmat4 viewMatrix = lookAt(dvec3(X, Y, 0), dvec3(0, 0, 0), dvec3(0, 0, 1));
	dmat4 projMatrix = perspective(M_PI / 4, double(windowWidth) / windowHeight, 0.1, 100.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixd(value_ptr(viewMatrix));
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixd(value_ptr(projMatrix));
	glMatrixMode(GL_MODELVIEW);
	drawGround();
	glBindTexture(GL_TEXTURE_2D, texture);

	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);

	glUseProgram(program);
	glBegin(GL_POINTS);
	for(int i = 0; i < NUM_PARTICLES; ++i)
	{
		FlameParticle const& p = particles[i];
		vec4 color = mix(p.colorEnd, p.colorFirst, p.life);
		glVertexAttrib1f(1, p.size);
		glVertexAttrib4f(2, color.r, color.g, color.b, p.life);
		glVertexAttrib1f(3, p.angle);
		glVertexAttrib3f(0, p.pos.x, p.pos.y, p.pos.z);
	}
	glEnd();
	
	glUseProgram(0);
	
	glDisable(GL_BLEND);

	glBindTexture(GL_TEXTURE_2D, 0);

	for(int i = 0; i < NUM_PARTICLES; ++i)
	{
		step(particles[i], 1.0 / 60.0);
	}   

	

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
