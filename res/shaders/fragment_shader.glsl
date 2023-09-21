#version 330 core
out vec4 FragColor;
  
in vec3 ourColor;
in vec2 TexCoord;
  
uniform sampler2D texture1;
uniform sampler2D texture2;

uniform float mixRatio;

void main()
{
    //FragColor = mix(texture(texture1, TexCoord), texture(texture2, vec2(1-TexCoord.x, TexCoord.y)), mixRatio);
    FragColor = vec4(1.0f);
}