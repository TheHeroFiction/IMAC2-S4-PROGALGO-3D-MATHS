#version 330 core

in vec3 vPostion_vs;
in vec3 vNormal_vs;
in vec2 vTexCoords_vs;

uniform sampler2D uTexture;
uniform sampler2D uTexture2;

out vec3 fFragColor;

void main()
{
    fFragColor = texture(uTexture,vTexCoords_vs).xyz + texture(uTexture2,vTexCoords_vs).xyz;
}