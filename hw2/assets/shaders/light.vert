#version 430

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;

uniform mat4 Projection;
uniform mat4 ViewMatrix;
uniform mat4 ModelMatrix;
// normal matrix
uniform mat4 ModelNormalMatrix;

out vec2 TexCoord;
// Normal of vertex in world space
out vec3 Normal;
// Position of vertex in world space
out vec3 FragPos;

// TODO#4-1: vertex shader / fragment shader
// Note:
//           1. how to write a vertex shader:
//              a. The output is gl_Position and anything you want to pass to the fragment shader. 
//                 (Apply matrix multiplication yourself)
//           2. how to write a fragment shader:
//              a. The output is FragColor (any var is OK)
//           3. Calculate
//              a. For direct light, lighting = ambient + diffuse + specular
//              b. For point light & spot light, lighting = ambient + attenuation * (diffuse + specular)
//              c. Final color = direct + point + spot if all three light are enabled
//           4. attenuation
//              a. Use formula from slides 'shading.ppt' page 20
//           5. For each light, ambient, diffuse and specular color are the same
//           6. diffuse = kd * max(normal vector dot light direction, 0.0)
//           7. specular = ks * pow(max(normal vector dot halfway direction), 0.0), shininess);
//           8. we've set all light parameters for you (see context.h) and definition in fragment shader
//           9. You should pre calculate normal matrix (trasfer normal from model local space to world space) 
//              in light.cpp rather than in shaders

void main() {
  gl_Position = vec4(0.0, 0.0, 0.0, 1.0);
  // TexCoord = vec2(0.0, 0.0);
  FragPos = vec3(0.0, 0.0, 0.0);
  Normal = vec3(0.0, 0.0, 0.0);

  gl_Position = ViewMatrix * Projection * ModelMatrix * vec4(position, 1.0);
  TexCoord = texCoord;
  FragPos = vec3(ModelMatrix * vec4(position, 1.0));
  Normal = normalize(normal);
}
