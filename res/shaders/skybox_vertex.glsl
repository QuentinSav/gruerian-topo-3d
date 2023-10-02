#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 TexCoords;

uniform mat4 projection;
uniform mat4 view_cubemap;

void main()
{
    TexCoords = aPos;
    gl_Position = projection * view_cubemap * vec4(aPos, 1.0);
} 