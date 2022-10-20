#version 410 core
layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexNormal;
layout(location = 2) in vec2 vertexUV;

out vec3 fragmentPosition;
out vec3 normalTransposed;
out vec2 UV;      //for textures
out vec3 VertNormal;

uniform mat4 mMatrix;   // objektet
uniform mat4 vMatrix;   // viewMatrisen, lokasjon og rotasjon til kameraet
uniform mat4 pMatrix;   // Projeksjons matrisen til kameraet, perspektivet til kameraet

void main()
{
   fragmentPosition = vec3(mMatrix * vec4(vertexPosition, 1.0));
   normalTransposed = mat3(transpose(inverse(mMatrix))) * vertexNormal;

   UV = vertexUV;      //for textures
   VertNormal = vertexNormal;
   gl_Position = pMatrix * vMatrix * mMatrix * vec4(vertexPosition, 1.0);
}
