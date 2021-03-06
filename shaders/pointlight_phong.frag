#version 450

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;

struct Light {
    vec3 position;
    vec3 color;
    float attenuation;
    float fallOff;
};
uniform Light light;
uniform vec3 eye;

out vec4 fragColor;

uniform samplerCube shadowMap;

bool inShadow(vec3 fragToLight, float fallOff) {
    float closestDepth = texture(shadowMap, fragToLight).r*fallOff;
    float currentDepth = length(fragToLight);
    return currentDepth - closestDepth > 0.05;
}

void main() {
    // retrieve data from G-buffer
    vec3 fragPos = texelFetch(gPosition, ivec2(gl_FragCoord.xy), 0).rgb;
    vec3 normal = texelFetch(gNormal, ivec2(gl_FragCoord.xy), 0).rgb;
    vec3 albedo = texelFetch(gAlbedoSpec, ivec2(gl_FragCoord.xy), 0).rgb;
    float spec = texelFetch(gAlbedoSpec, ivec2(gl_FragCoord.xy), 0).a;

    vec3 fragToLight = fragPos - light.position;
    if (length(fragToLight) > light.fallOff || inShadow(fragToLight, light.fallOff)) {
        discard;
    }

    vec3 viewDir = normalize(eye - fragPos);
    vec3 lightDir = normalize(light.position - fragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 lightColor = light.color/(light.attenuation*dot(light.position - fragPos, light.position - fragPos));

    vec3 diffuse = max(dot(normal, lightDir), 0.0) * albedo * lightColor;
    vec3 specular = pow(max(dot(viewDir, reflectDir), 0.0), 5.0) * spec * albedo * lightColor;
    fragColor = vec4(diffuse + specular, 1.0);
}
