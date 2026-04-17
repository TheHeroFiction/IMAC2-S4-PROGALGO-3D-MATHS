#version 330 core

in vec3 vPostion_vs;
in vec3 vNormal_vs;
in vec2 vTexCoords_vs;

uniform vec3 uTexture;

out vec3 fFragColor;

void main()
{
    fFragColor = uTexture;
}