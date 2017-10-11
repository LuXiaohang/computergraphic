#version 150 compatibility

in vec3 p_pos;
in float p_size;
in vec4 p_color;
in float p_rotation;

out vec3 pos;
out float size;
out vec4 color;
out float rotation;

void main()
{
    pos = p_pos;
    size = p_size;
    color = p_color;
    rotation = p_rotation;
}
