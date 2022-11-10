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


vec3 CalcPointLight(Material material, PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, sampler2D ourTexture){

    vec3 lightDir = normalize(light.position - fragPos);
    vec3 norm = normalize(normal);

    // diffuse shading
    float diff = max(dot(norm, -lightDir), 0.0);

    // specular shading
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    // attenuation
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    

    // combine results
    vec3 ambient  = material.ambient  * vec3(texture(ourTexture, TexCoord));
    vec3 diffuse  = material.diffuse  * diff * vec3(texture(ourTexture, TexCoord));
    vec3 specular = material.specular * spec * vec3(texture(ourTexture, TexCoord));

    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;

    return (light.lightColor * (ambient + diffuse + specular));
} 


void main() {
    //color = vec4(0.0, 0.0, 0.0, 1.0);
    color =  texture2D(ourTexture, TexCoord);

    vec3 viewDir = normalize(viewPos - FragPos);

    if(dl.enable == 1){
        vec3 DL_norm = normalize(Normal);
        float DL_diff = max(dot(DL_norm, -dl.direction), 0.0);
        vec3 DL_diffuse = material.diffuse * DL_diff;
        
        vec3 DL_reflectDir = reflect(-dl.direction, DL_norm);
        float DL_spec = pow(max(dot(viewDir, DL_reflectDir), 0.0), material.shininess);
        vec3 DL_specular = material.specular * DL_spec;

        vec3 DL_result = dl.lightColor * (material.ambient + DL_diffuse + DL_specular);

        color =  texture2D(ourTexture, TexCoord);
        color = color * vec4(DL_result, 1.0);
    }

    if(pl.enable == 1){
        color =  texture2D(ourTexture, TexCoord);
        color = color * vec4(CalcPointLight(material, pl, Normal, FragPos, viewDir, ourTexture), 1.0);
    }
}
