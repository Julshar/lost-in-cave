#version 440 core

out vec4 FragColor;

in vec4 vertexColor;
in vec2 TexCoord;

uniform vec4 uniformColor;

uniform sampler2D texture1; // texture unit 1
uniform sampler2D texture2; // texture unit 2

void main()
{
  FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.5);
} 