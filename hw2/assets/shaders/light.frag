#version 430

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

out vec4 color;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
}; 

struct DirectionLight {
    int enable;
    vec3 direction;
    vec3 lightColor;
};

struct PointLight {
    int enable;
    vec3 position;  
    vec3 lightColor;

    float constant;
    float linear;
    float quadratic;
};

struct Spotlight {
    int enable;
    vec3 position;
    vec3 direction;
    vec3 lightColor;
    float cutOff;

    // Paramters for attenuation formula
    float constant;
    float linear;
    float quadratic;      
}; 

uniform sampler2D ourTexture;
uniform vec3 viewPos;

uniform Material material;
uniform DirectionLight dl;
uniform PointLight pl;
uniform Spotlight sl;

void main() {
    //color = vec4(0.0, 0.0, 0.0, 1.0);
    color =  texture2D(ourTexture, TexCoord);

    vec3 ambient = material.ambient;

    if(dl.enable == 1){
        vec3 norm = normalize(Normal);
        float diff = max(dot(norm, -dl.direction), 0.0);
        vec3 diffuse = material.diffuse * diff;

        vec3 viewDir = normalize(viewPos - FragPos);
        vec3 reflectDir = reflect(-dl.direction, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
        vec3 specular = material.specular * spec;

        vec3 result = dl.lightColor * (ambient + diffuse + specular);
        color = color * vec4(result, 1.0);
    }
}
