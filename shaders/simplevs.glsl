#version 330 core
layout (location = 0) in vec3 inPosition; 
layout (location = 1) in vec2 inTexCoord; 

uniform vec3 uMove;

uniform mat4 uTransform;
uniform mat3 uMtxTransform;

out vec2 texCoord;
void main()
{      
     gl_Position = uTransform*vec4(uMtxTransform*inPosition+uMove, 1.0);
     texCoord = inTexCoord;
}

    