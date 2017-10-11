#version 150 compatibility

uniform sampler2D tex;

in vec2 texCoord;
in vec4 color_f;

void main()
{
    gl_FragColor.rgb = color_f.rgb;
    gl_FragColor.a = texture(tex, texCoord).r * color_f.a;
}
