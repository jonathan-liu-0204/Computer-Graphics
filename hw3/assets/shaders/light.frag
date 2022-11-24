#version 430

#define shininess 10

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

out vec4 color;

uniform sampler2D ourTexture;

uniform vec3 viewPos;

struct DirectionLight {
    vec3 direction;  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform DirectionLight dl;

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

    vec3 total = ambient + diffuse + specular;
    total = clamp(total, vec3(0, 0, 0), vec3(1, 1, 1));
        
    vec3 result = total * texture(ourTexture, TexCoord).xyz;
    color = vec4(result, 1.0f);
}
