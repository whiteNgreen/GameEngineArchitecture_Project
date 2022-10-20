#version 410 core
layout(location = 0) in vec4 positionIn;
layout(location = 1) in vec4 colorIn;
layout(location = 2) in vec2 vertexUV;

out vec4 colorOut;
out vec2 UV;
uniform mat4 mMatrix;
uniform mat4 vMatrix;
uniform mat4 pMatrix;

void main() {
    colorOut = colorIn;
    UV = vertexUV;
    gl_Position = pMatrix * vMatrix * mMatrix * positionIn;
}
