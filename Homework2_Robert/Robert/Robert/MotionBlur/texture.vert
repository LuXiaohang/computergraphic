#version 120

uniform vec3 center;

varying vec2 texcoord;

void main()
{
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}
