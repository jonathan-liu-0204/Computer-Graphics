#version 430

#define shininess 10

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;
in vec4 LightFragPost;

out vec4 color;

uniform sampler2D ourTexture;
uniform sampler2D shadowMap;

uniform vec3 viewPos;
uniform vec3 fakeLightPos;

uniform int enableShadow;

struct DirectionLight {
    vec3 direction;  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform DirectionLight dl;

float ShadowCalculation(){
    float bias = 0.002;
    // TODO
    
    vec3 projCoords = LightFragPost.xyz / LightFragPost.w;
    projCoords = projCoords * 0.5 + 0.5;

    float closestDepth = texture(shadowMap, projCoords.xy).r;
    float currentDepth = projCoords.z;

    vec3 lightDir = normalize(FragPos - fakeLightPos);

    float shadow = currentDepth-bias > closestDepth  ? 1.0 : 0.0;

    if(currentDepth > 1.0){
        shadow = 0.0;
    }

    return shadow;
}

void main() {
    vec3 ambient = dl.ambient;

    // Diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(-dl.direction);  
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = dl.diffuse * diff;
    
    // Specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = dl.specular * spec;

    float shadow = enableShadow > 0 ? ShadowCalculation() : 0.0;
    vec3 total = (ambient + (1.0 - shadow) * (diffuse + specular));
    total = clamp(total, vec3(0, 0, 0), vec3(1, 1, 1));
        
    vec3 result = total * texture(ourTexture, TexCoord).xyz;
    color = vec4(result, 1.0f);
}
