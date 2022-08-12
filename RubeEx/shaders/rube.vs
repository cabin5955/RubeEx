#version 300 es
layout(location = 0) in vec2 a_position;
layout(location = 1) in vec2 a_texcoords;

out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    float u = a_texcoords.x;
    float v = a_texcoords.y;
    v = 1.0 - a_texcoords.y;
    
    TexCoords = vec2(u,v);
    gl_Position = projection * view * model * vec4(a_position.xy, 0.0, 1.0);
}
