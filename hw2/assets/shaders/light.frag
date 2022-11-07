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
    float ks = 0.75;
    float kd = 0.75;

    color = vec4(0.0, 0.0, 0.0, 1.0);
    //color = texture2D(ourTexture, TexCoord);

    if(dl.enable == 1){
       // material.diffuse = kd * max(dot(Normal, dl.direction), 0.0);
       material.specular = ks * pow(max(dot(Normal, (dl.direction/2)), 0.0), material.shininess);
       dl.lightColor = material.ambient + (kd * max(dot(Normal, dl.direction), 0.0)) + material.specular;
    }

    if(pl.enable == 1){

    }

    if(sl.enable == 1){
    
    }

    color = (dl.lightColor, 1.0);
}
