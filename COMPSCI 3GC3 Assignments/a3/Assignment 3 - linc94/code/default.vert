#version 330 core

// set layout
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

// set uniforms
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 Normal;
out vec2 TexCoord;
out vec3 FragPos;

void main()
{
    // calculate position
    gl_Position = projection * view * model * vec4(aPos, 1.0f);

    // normalize
    Normal = normalize(mat3(model) * aNormal); 

    TexCoord = aTexCoord;
    FragPos = vec3(model * vec4(aPos, 1.0));
}

// Task 1
// #version 330 core
// layout (location = 0) in vec3 aPos;
// layout (location = 1) in vec3 aNormal;
// layout (location = 2) in vec2 aTexCoord;

// uniform mat4 model;
// uniform mat4 view;
// uniform mat4 projection;

// out vec3 Normal; // declare an output normal
// out vec2 TexCoord;

// void main()
// {
    // gl_Position = projection * view * model * vec4(aPos, 1.0f);
    // Normal = normalize(mat3(model) * aNormal); // Transofrm the noraml to world space and assigne the output variable
    // TexCoord = aTexCoord;
// }