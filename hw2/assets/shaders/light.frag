#version 430

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

out vec4 color;

uniform sampler2D ourTexture;

uniform vec3 viewPos;

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

uniform Material material;
uniform DirectionLight dl;
uniform PointLight pl;
uniform Spotlight sl;

void main() {
    //color = vec4(0.0, 0.0, 0.0, 1.0);
    color = texture2D(ourTexture, TexCoord);

   if(dl.enable == 1){
        // vec3 diffuse = material.diffuse * max(dot(Normal, dl.direction), 0.0);
        // vec3 specular = material.specular * pow(max(dot(Normal, dl.direction / 2), 0.0), material.shininess);
        // vec3 tmp_dl_lightColor = dl.lightColor;
        // tmp_dl_lightColor = tmp_dl_lightColor + material.ambient + diffuse + specular;
        // color = color + vec4(tmp_dl_lightColor, 0.0);
         color = texture2D(ourTexture, TexCoord);
    }
}
