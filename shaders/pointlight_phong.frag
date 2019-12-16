#version 430

in vec2 vTexCoord;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;

struct Light {
    vec3 position;
    vec3 color;
    float attenuation;
};
uniform Light light;
uniform vec3 eye;

out vec4 fragColor;

void main() {
    // retrieve data from G-buffer
    vec3 fragPos = texture(gPosition, vTexCoord).rgb;
    vec3 normal = texture(gNormal, vTexCoord).rgb;
    vec3 albedo = texture(gAlbedoSpec, vTexCoord).rgb;
    float spec = texture(gAlbedoSpec, vTexCoord).a;

    vec3 viewDir = normalize(eye - fragPos);
    vec3 lightDir = normalize(light.position - fragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 lightColor = light.color/(light.attenuation*distance(light.position, fragPos));

    vec3 ambient = albedo * 0.1;// hard-coded ambient component
    vec3 diffuse = max(dot(normal, lightDir), 0.0) * albedo * lightColor;
    vec3 specular = pow(max(dot(viewDir, reflectDir), 0.0), 5.0) * spec * albedo * lightColor;
    fragColor = vec4(ambient + diffuse + specular, 1.0);
}
