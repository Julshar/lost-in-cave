#version 440 core

uniform vec4 color;
uniform sampler2D textureSampler;

in vec2 textureCoordinate_frag;

out vec4 FragmentColor;

void main()
{
  FragmentColor = texture(textureSampler, textureCoordinate_frag) * color;
}