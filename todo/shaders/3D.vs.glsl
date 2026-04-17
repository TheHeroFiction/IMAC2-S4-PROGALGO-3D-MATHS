#version 330 core

// Attributs de sommet
layout(location = 0) in vec3 aVertexPosition; // Position du sommet
layout(location = 1) in vec3 aVertexNormal; // Normale du sommet
layout(location = 2) in vec2 aVertexTexCoords; // Coordonnées de texture du sommet

// Matrices de transformations reçues en uniform
uniform mat4 uMVPMatrix;
uniform mat4 uMVMatrix;
uniform mat4 uNormalMatrix;

// Sorties du shader
out vec3 vPostion_vs;
out vec3 vNormal_vs;
out vec2 vTexCoords_vs;

void main()
{
    // Passage coordonnées homogènes
    vec4 vertexPostion = vec4(aVertexPosition,1);
    vec4 vertexNormal = vec4(aVertexNormal,0);

    // Calcul des valeurs de Sorties
    vPostion_vs = vec3(uMVMatrix * vertexPostion);
    vNormal_vs = vec3(uNormalMatrix * vertexNormal);
    vTexCoords_vs = aVertexTexCoords;

    // Calcul position projetée
    gl_Position = uMVPMatrix * vertexPostion;
}