 #define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <GL/glew.h>
#include <GL/glut.h>
#pragma comment(lib,"glew32.lib")
#pragma comment(lib,"glut32.lib")
 
GLuint v,f,p;
 
static float shoulder =-20,elbow =60,hand =10;
 
void init(void)
{
 
    glClearColor(0.0,0.0,0.0,0.0);
    glShadeModel(GL_FLAT);
 
}
 
char *readShaderFile(const char *fileName )
{
 
    FILE *fp;
    char *content = NULL;
    int count=0;
    if (fileName != NULL) {
 
        fp = fopen(fileName,"rt");
        if (fp != NULL) {
 
            fseek(fp, 0, SEEK_END);
            count = ftell(fp);
            rewind(fp);
            if (count > 0) {
 
                content = (char *)malloc(sizeof(char) * (count+1));
                count = fread(content,sizeof(char),count,fp);
                content[count] = '\0';
             
}
            fclose(fp);
         
}
     
}
    return content;
 
}
 
void initShaders() {
 
    char *vs = NULL,*fs = NULL;
    v = glCreateShader(GL_VERTEX_SHADER);
    f = glCreateShader(GL_FRAGMENT_SHADER);
    vs = readShaderFile("texture.vert");
    fs = readShaderFile("texture.frag");
    const char * ff = fs;
    const char * vv = vs;
    glShaderSource(v,1,&vv,NULL);
    glShaderSource(f,1,&ff,NULL);
    free(vs);
    free(fs);
    glCompileShader(v);
    glCompileShader(f);
    p = glCreateProgram();
    glAttachShader(p,f);
    glAttachShader(p,v);
    glLinkProgram(p);
    glUseProgram(p);
 
}
 
//使用着色器进行顶点的平移、旋转、拉伸等变换
void useShader(float s11,float s12,float s13,
               float t11=0,float t12=0,float t13=0,float r1=0,
               float t21=0,float t22=0,float t23=0,float r2=0,
               float t31=0,float t32=0,float t33=0,float r3=0,
               float t41=0,float t42=0,float t43=0,float r4=0,
               float t51=0,float t52=0,float t53=0)
{
 
    GLint loc;
    //拉伸变换参数
    loc =glGetUniformLocation(p,"s11");
    glUniform1f(loc,(GLfloat)s11);
    loc =glGetUniformLocation(p,"s12");
    glUniform1f(loc,(GLfloat)s12);
    loc =glGetUniformLocation(p,"s13");
    glUniform1f(loc,(GLfloat)s13);
    //第一次平移变换参数
    loc =glGetUniformLocation(p,"t11");
    glUniform1f(loc,(GLfloat)t11);
    loc =glGetUniformLocation(p,"t12");
    glUniform1f(loc,(GLfloat)t12);
    loc =glGetUniformLocation(p,"t13");
    glUniform1f(loc,(GLfloat)t13);
    //第一次旋转变换参数
    loc =glGetUniformLocation(p,"r1");
    glUniform1f(loc,(GLfloat)r1);
    //第二次平移变换参数
    loc =glGetUniformLocation(p,"t21");
    glUniform1f(loc,(GLfloat)t21);
    loc =glGetUniformLocation(p,"t22");
    glUniform1f(loc,(GLfloat)t22);
    loc =glGetUniformLocation(p,"t23");
    glUniform1f(loc,(GLfloat)t23);
    //第二次旋转变换参数
    loc =glGetUniformLocation(p,"r2");
    glUniform1f(loc,(GLfloat)r2);
    //第三次平移变换参数
    loc =glGetUniformLocation(p,"t31");
    glUniform1f(loc,(GLfloat)t31);
    loc =glGetUniformLocation(p,"t32");
    glUniform1f(loc,(GLfloat)t32);
    loc =glGetUniformLocation(p,"t33");
    glUniform1f(loc,(GLfloat)t33);
    //第三次旋转变换参数
    loc =glGetUniformLocation(p,"r3");
    glUniform1f(loc,(GLfloat)r3);
    //第四次平移变换参数
    loc =glGetUniformLocation(p,"t41");
    glUniform1f(loc,(GLfloat)t41);
    loc =glGetUniformLocation(p,"t42");
    glUniform1f(loc,(GLfloat)t42);
    loc =glGetUniformLocation(p,"t43");
    glUniform1f(loc,(GLfloat)t43);
    //第四次旋转变换参数
    loc =glGetUniformLocation(p,"r4");
    glUniform1f(loc,(GLfloat)r4);
    //第五次平移变换参数
    loc =glGetUniformLocation(p,"t51");
    glUniform1f(loc,(GLfloat)t51);
    loc =glGetUniformLocation(p,"t52");
    glUniform1f(loc,(GLfloat)t52);
    loc =glGetUniformLocation(p,"t53");
    glUniform1f(loc,(GLfloat)t53);
 
}
 
void display(void) {
 
 
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //躯干
    useShader(1.6,2.8,0.8,-1.5,-1.2,0.0);
    glutWireCube(1.0);
    //上臂
    useShader(1.4,0.4,0.46,0.7,-0.2,0.0,shoulder,-0.7,0.2,0.0);
    glutWireCube(1.0);
    //小臂
    useShader(1.4,0.4,0.46,0.7,-0.2,0.0,elbow,1.4,0.0,0.0,shoulder,-0.7,0.2,0.0);
    glutWireCube(1.0);
    //5根手指
    useShader(0.2,0.1,0.1,0.1,-0.05,-0.18,hand,1.4,0.0,0.0,elbow,1.4,0.0,0.0,shoulder,-0.7,0.2,0.0);
    glutWireCube(1.0);
    useShader(0.2,0.1,0.1,0.1,-0.05,-0.06,hand,1.4,0.0,0.0,elbow,1.4,0.0,0.0,shoulder,-0.7,0.2,0.0);
    glutWireCube(1.0);
    useShader(0.2,0.1,0.1,0.1,-0.05,0.06,hand,1.4,0.0,0.0,elbow,1.4,0.0,0.0,shoulder,-0.7,0.2,0.0);
    glutWireCube(1.0);
    useShader(0.2,0.1,0.1,0.1,-0.05,0.18,hand,1.4,0.0,0.0,elbow,1.4,0.0,0.0,shoulder,-0.7,0.2,0.0);
    glutWireCube(1.0);
    useShader(0.2,0.1,0.1,0.1,0.05,0.18,-hand,1.4,-0.4,0.0,elbow,1.4,0.0,0.0,shoulder,-0.7,0.2,0.0);
    glutWireCube(1.0);
    //5根手指前段
    useShader(0.2,0.1,0.1,0.1,0.05,-0.18,-45,0.2,-0.1,0.0,hand,1.4,0.0,0.0,elbow,1.4,0.0,0.0,shoulder,-0.7,0.2,0.0);
    glutWireCube(1.0);
    useShader(0.2,0.1,0.1,0.1,0.05,-0.06,-45,0.2,-0.1,0.0,hand,1.4,0.0,0.0,elbow,1.4,0.0,0.0,shoulder,-0.7,0.2,0.0);
    glutWireCube(1.0);
    useShader(0.2,0.1,0.1,0.1,0.05,0.06,-45,0.2,-0.1,0.0,hand,1.4,0.0,0.0,elbow,1.4,0.0,0.0,shoulder,-0.7,0.2,0.0);
    glutWireCube(1.0);
    useShader(0.2,0.1,0.1,0.1,0.05,0.18,-45,0.2,-0.1,0.0,hand,1.4,0.0,0.0,elbow,1.4,0.0,0.0,shoulder,-0.7,0.2,0.0);
    glutWireCube(1.0);
    useShader(0.2,0.1,0.1,0.1,-0.05,0.18,45,0.2,0.1,0.0,-hand,1.4,-0.4,0.0,elbow,1.4,0.0,0.0,shoulder,-0.7,0.2,0.0);
    glutWireCube(1.0);
 
    glutSwapBuffers();
 
}
 
void reshape(int w,int h)
{
 
    glViewport(0,0,(GLsizei)w,(GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(65.0,(GLfloat)w/(GLfloat)h,1.0,20.0);
    gluLookAt(0.0,0.5,5.0,
              0.0,0.0,0.0,
              0.0,1.0,0.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
 
}
 
void keyboard(unsigned char key, int x, int y)
{
 
    switch (key){
 
        case 's':
            if(shoulder<80){
 
                shoulder =shoulder +5;
                glutPostRedisplay();
             
}
            break;
        case 'S':
            if(shoulder>-80){
 
                shoulder =shoulder -5;
                glutPostRedisplay();
             
}
            break;
        case 'e':
            if(elbow<160){
 
                elbow =elbow +5;
                glutPostRedisplay();
             
}
            break;
        case 'E':
            if(elbow>-160){
 
                elbow =elbow -5;
                glutPostRedisplay();
             
}
            break;
        case 'h':
            if(hand<90){
 
                hand =hand +5;
                glutPostRedisplay();
             
}
            break;
        case 'H':
            if(hand>5){
 
                hand =hand -5;
                glutPostRedisplay();
             
}
            break;
        default:
            break;
     
}
 
}
 
int main(int argc, char **argv) {
 
    printf("Press 's' to turn the shoulder up and 'S' to turn it down.\n");
    printf("Press 'e' to turn the elbow up and 'E' to turn it down.\n");
    printf("Press 'h' to open the hand and 'H' to close it.\n");
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100,100);
    glutInitWindowSize(500,500);
    glutInitWindowPosition(100,100);
    glutCreateWindow(argv[0]);
    init();
    glewInit();
    initShaders();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutMainLoop();
    return 0;
 
}