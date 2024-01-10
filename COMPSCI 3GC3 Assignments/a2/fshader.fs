//#version 330 core
//out vec4 FragColor;  
//in vec3 ourColor;
  
//void main()
//{
    //FragColor = vec4(ourColor, 1.0);
//}

#version 330 core

in vec3 Normal;
out vec3 FragColor;

void main() {
    float c = dot(Normal, vec3(0.8,0.7,0.6));
    FragColor = vec3(c,c,c);
}