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
// uniform PointLight pl;
// uniform Spotlight sl;

vec3 diffuse;
vec3 specular;
vec3 tmp_dl_lightColor;
vec3 viewDir;
vec3 halfwayDir;

void main() {
    //color = vec4(0.0, 0.0, 0.0, 1.0);
    color = texture2D(ourTexture, TexCoord);

    viewDir = normalize(viewPos - FragPos);

   if(dl.enable == 1){
       halfwayDir = normalize(dl.direction + viewDir);
       diffuse = dl.lightColor * max(dot(Normal, dl.direction), 0.0);
       specular = material.specular * pow(max(dot(Normal, halfwayDir), 0.0), material.shininess);
       tmp_dl_lightColor = dl.lightColor + material.ambient + diffuse + specular;
       // color = color + vec4(tmp_dl_lightColor, 1.0);
       color = 
   }
}
