#version 430

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;

uniform mat4 Projection;
uniform mat4 ViewMatrix;
uniform mat4 ModelMatrix;
uniform mat4 TIModelMatrix;
uniform mat4 LightViewMatrix;

out vec2 TexCoord;
// Normal of vertex in world space
out vec3 Normal;
// Position of vertex in world space
out vec3 FragPos;
// Position of vertex in light view space
out vec4 LightFragPost;

// TODO#2-4: shadow-enabled shader with single direct light
//           1. Finish main to pass variables to fragment shader
//           2. Finish ShadowCalculation to check whether the fragment is in shadow
//              (return 1.0 if in shadow or 0.0)
// Note:     
//           Transfer the fragment to light's view and store in LightFragPost
//           Check the depth of the fragment (z). if greater then depth map value,
//           the fragment is in shadow.
//           You need to apply static bais 0.002 to solve shadow acne
//           If the frament is further than the light's far plane 
//           when its z coordinate is larger than 1.0, you should return 0

void main() {
	gl_Position = Projection * ViewMatrix * ModelMatrix * vec4(position, 1.0);
	FragPos = vec3(ModelMatrix * vec4(position, 1.0));
	Normal = mat3(TIModelMatrix) * normal;
	TexCoord = texCoord;
	LightFragPost = LightViewMatrix * vec4(FragPos, 1.0);
}
