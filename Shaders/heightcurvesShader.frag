#version 410 core
out vec4 fragColor;

in vec3 fragmentPosition;

uniform vec3 curveColor = vec3(1,1,1);

uniform float heightStep = 1.f;
uniform float maxHeight = 50.f;
uniform float lineSize = 0.01f;

void main()
{
    fragColor = vec4(curveColor, 0);

    /* For loop som går igjennom høyden */
    for (float i = 0; i < maxHeight; i += heightStep)
    {
        if (fragmentPosition.z < (i + lineSize) && fragmentPosition.z > i)
        {
            fragColor = vec4(curveColor, 1);
        }
    }
}
