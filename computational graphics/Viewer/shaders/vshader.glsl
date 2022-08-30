#version 330 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal; 
layout(location = 2) in vec2 texCoords;


// The model/view/projection matrices
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 modelColor;

// needed for final color.
out vec3 fragColor;
// needed to calculate the shading.
out vec3 fragPos;
out vec3 normalPos;
// textures.
out vec2 fragTexCoords;

void main()
{
	fragTexCoords = texCoords;
	// calculating normal using the model mat.
    normalPos = mat3(transpose(inverse(model))) * normal;
	// frag pos
	fragPos = vec3( view * model* vec4( pos, 1 ) );
	// model color.
	fragColor = modelColor;
    gl_Position = projection * view *  model * vec4(pos, 1.0f);

}
