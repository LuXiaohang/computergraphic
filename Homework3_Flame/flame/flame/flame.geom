#version 150 compatibility

layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

in vec3 pos[1];
in float size[1];
in vec4 color[1];
in float rotation[1];

out vec2 texCoord;
out vec4 color_f;

void genVertex(vec3 eyeCoord, vec2 X, vec2 Y, float size, vec2 quadCoord)
{
    vec3 p = vec3(eyeCoord.xy + mat2(X, Y) * (size * (quadCoord - 0.5)), eyeCoord.z);
    gl_Position = gl_ProjectionMatrix * vec4(p, 1.0);
    texCoord = quadCoord;
    color_f = color[0];
    EmitVertex();
}

void main()
{
    vec3 eyeCoord = (gl_ModelViewMatrix * vec4(pos[0], 1.0)).xyz;
    vec2 X = vec2( cos(rotation[0]), sin(rotation[0]));
    vec2 Y = vec2(-sin(rotation[0]), cos(rotation[0]));
    genVertex(eyeCoord, X, Y, size[0], vec2(0, 1));
    genVertex(eyeCoord, X, Y, size[0], vec2(0, 0));
    genVertex(eyeCoord, X, Y, size[0], vec2(1, 1));
    genVertex(eyeCoord, X, Y, size[0], vec2(1, 0));
}
