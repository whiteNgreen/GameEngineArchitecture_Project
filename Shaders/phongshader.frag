#version 410 core
out vec4 fragColor;     //The final color

in vec3 normalTransposed;
in vec3 fragmentPosition;
in vec2 UV;
in vec3 VertNormal;

uniform sampler2D textureSampler;

uniform float ambientStrength = 0.1;

uniform vec3 lightPosition;
uniform vec3 lightColor = vec3(.3, .3, 1);
uniform float lightStrength = 0.9f;
uniform float specularStrength = 0.9;
uniform int specularExponent = 32;

uniform vec3 objectColor = vec3(1.0, 1.0, 1.0);

uniform vec3 cameraPosition;

uniform int UsingTextures;

void main()
{
    vec3 ambient = ambientStrength * lightColor;

    // Diffuse
    vec3 normalCorrected = normalize(normalTransposed);
    vec3 lightDirection = normalize(lightPosition - fragmentPosition);
    float angleFactor = max(dot(normalCorrected, lightDirection), 0.0);
    vec3 diffuse = angleFactor * objectColor * lightColor * lightStrength;

    // Specular
    vec3 viewDirection = normalize(cameraPosition - fragmentPosition);
    float spec = 0.0;
    if (angleFactor > 0.0)
    {
        vec3 reflectDirection = reflect(-lightDirection, normalCorrected);
        spec = pow(max(dot(viewDirection, reflectDirection), 0.0), specularExponent) * lightStrength;
    }
    vec3 specular = spec * lightColor * specularStrength;

    vec3 result = ambient + diffuse + specular;

    if (UsingTextures == 1)
    {
        fragColor = vec4(result, 1.0) * texture(textureSampler, UV);
    }
    else
    {
        fragColor = vec4(result, 1.0) * vec4(VertNormal, 1.0);

        /* Testing av HoydeKurver med shader. Alpha culler ut deler av modellen som ikke har krevd hoyde */
//        float height = 6.f;
//        if (fragmentPosition.z > height + 0.1 || fragmentPosition.z < height - 0.1)
//        {
//            fragColor = vec4(1,1,1,0);
//        }
    }
}
