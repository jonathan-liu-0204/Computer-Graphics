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

vec4 CalcDirLight(){
   
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 lightDir = normalize(-dl.direction);
    vec3 reflectDir = reflect(-lightDir, norm);

    float diff = max(dot(norm, lightDir), 0.0);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    vec4 ambient  = vec4(dl.lightColor * material.ambient, 1.0)  * texture(ourTexture, TexCoord);
    vec4 diffuse  = vec4(dl.lightColor * material.diffuse * diff, 1.0) * texture(ourTexture, TexCoord);
    vec4 specular = vec4(dl.lightColor * material.specular * spec, 1.0) * texture(ourTexture, TexCoord);

    return (ambient + diffuse + specular);
}  

vec4 CalcPointLight(){
    
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(pl.position - FragPos);
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    
    float diff = max(dot(norm, lightDir), 0.0);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    float distance = length(pl.position - FragPos);
    float attenuation = 1.0 / (pl.constant + pl.linear * distance + pl.quadratic * (distance * distance));  
                 
    vec4 ambient  = vec4(pl.lightColor * material.ambient, 1.0)  * texture(ourTexture, TexCoord);
    vec4 diffuse  = vec4(pl.lightColor * material.diffuse * diff, 1.0) * texture(ourTexture, TexCoord);
    vec4 specular = vec4(pl.lightColor * material.specular * spec, 1.0) * texture(ourTexture, TexCoord);

    diffuse  *= attenuation;
    specular *= attenuation;

    return (ambient + diffuse + specular);
}

vec4 CalcSpotLight(){

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(sl.position - FragPos);
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);

    float diff = max(dot(norm, lightDir), 0.0);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    float distance = length(sl.position - FragPos);
    float attenuation = 1.0 / (sl.constant + sl.linear * distance + sl.quadratic * (distance * distance));    

    float theta = dot(lightDir, normalize(-sl.direction)); 

    vec4 ambient = vec4(sl.lightColor * material.ambient, 1.0) * texture(ourTexture, TexCoord);
    vec4 diffuse = vec4(sl.lightColor * material.diffuse * diff, 1.0) * texture(ourTexture, TexCoord);
    vec4 specular = vec4(sl.lightColor * material.specular * spec, 1.0) * texture(ourTexture, TexCoord);

    diffuse *= attenuation;
    specular *= attenuation;

    if(theta > sl.cutOff){
        return (ambient + diffuse + specular);
    }
    else{
        return vec4(sl.lightColor * material.ambient, 1.0) * texture(ourTexture, TexCoord);
    }
}

void main() {
    color = vec4(0.0, 0.0, 0.0, 1.0);

    if(dl.enable == 1){
        color += CalcDirLight();
    }

    if(pl.enable == 1){
        color += CalcPointLight();
    }

    if(sl.enable == 1){
        color += CalcSpotLight();
    }

    if(dl.enable == 0 && pl.enable == 0 && sl.enable == 0){
        color = vec4(material.ambient, 1.0) * texture(ourTexture, TexCoord);
    }
}
