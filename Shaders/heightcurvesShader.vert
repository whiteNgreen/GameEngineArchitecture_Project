#version 410 core
layout(location = 0) in vec3 vertexPosition;
//layout(location = 1) in vec3 vertexNormal;
//layout(location = 2) in vec2 vertexUV;

out vec3 fragmentPosition;

uniform mat4 mMatrix;   // objektet
uniform mat4 vMatrix;   // viewMatrisen, lokasjon og rotasjon til kameraet
uniform mat4 pMatrix;   // Projeksjons matrisen til kameraet, perspektivet til kameraet

void main()
{
   fragmentPosition = vec3(mMatrix * vec4(vertexPosition, 1.0));

   gl_Position = pMatrix * vMatrix * mMatrix * vec4(vertexPosition, 1.0);
}
