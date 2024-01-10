#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <fstream>
#include <string>
#include "shader.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// get sun revolution degree/day
float get_sun_rotate_angle_around_itself(float day) {
    return float((360.0 / 27.0) * day);
}

// get earth orbit degree/day
float get_earth_rotate_angle_around_sun(float day) {
    return float((360.0 / 365.0) * day);
}

// get earth revolution degree/day
float get_earth_rotate_angle_around_itself(float day) {
    return float((360.0 / 1.0) * day);
}

// get moon orbit degree/day
float get_moon_rotate_angle_around_earth(float day) {
    return float((360.0 / 28.0) * day);
}

// get moon revolution degree/day
float get_moon_rotate_angle_around_itself(float day) {
    return float((360.0 / 28.0) * day);
}

// screen size settings
const unsigned int SCR_WIDTH = 1024;
const unsigned int SCR_HEIGHT = 768;

int main()
{

    // initialize and configure glfw
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    // glfw window creation
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    glEnable(GL_DEPTH_TEST);

    // build and compile our shader zprogram
    Shader ourShader("vshader.vs", "fshader.fs");

    // vertex data and vertex colour
    float vertices[] = {
        // front face, yellow
        -1.0f, -1.0f, -1.0f,  1.0f, 1.0f, 0.0f,
         1.0f, -1.0f, -1.0f,  1.0f, 1.0f, 0.0f,
         1.0f,  1.0f, -1.0f,  1.0f, 1.0f, 0.0f,
         1.0f,  1.0f, -1.0f,  1.0f, 1.0f, 0.0f,
        -1.0f,  1.0f, -1.0f,  1.0f, 1.0f, 0.0f,
        -1.0f, -1.0f, -1.0f,  1.0f, 1.0f, 0.0f,

        // back face, fuschia
        -1.0f, -1.0f,  1.0f,  1.0f, 0.0f, 1.0f,
         1.0f, -1.0f,  1.0f,  1.0f, 0.0f, 1.0f,
         1.0f,  1.0f,  1.0f,  1.0f, 0.0f, 1.0f,
         1.0f,  1.0f,  1.0f,  1.0f, 0.0f, 1.0f,
        -1.0f,  1.0f,  1.0f,  1.0f, 0.0f, 1.0f,
        -1.0f, -1.0f,  1.0f,  1.0f, 0.0f, 1.0f,

        // left face, red
        -1.0f,  1.0f,  1.0f,  1.0f, 0.0f, 0.0f,
        -1.0f,  1.0f, -1.0f,  1.0f, 0.0f, 0.0f,
        -1.0f, -1.0f, -1.0f,  1.0f, 0.0f, 0.0f,
        -1.0f, -1.0f, -1.0f,  1.0f, 0.0f, 0.0f,
        -1.0f, -1.0f,  1.0f,  1.0f, 0.0f, 0.0f,
        -1.0f,  1.0f,  1.0f,  1.0f, 0.0f, 0.0f,

        // right face, green
        1.0f,  1.0f,  1.0f,  0.0f, 1.0f, 0.0f,
        1.0f,  1.0f, -1.0f,  0.0f, 1.0f, 0.0f,
        1.0f, -1.0f, -1.0f,  0.0f, 1.0f, 0.0f,
        1.0f, -1.0f, -1.0f,  0.0f, 1.0f, 0.0f,
        1.0f, -1.0f,  1.0f,  0.0f, 1.0f, 0.0f,
        1.0f,  1.0f,  1.0f,  0.0f, 1.0f, 0.0f,

        // bottom face, aqua
       -1.0f, -1.0f, -1.0f,  0.0f, 1.0f, 1.0f,
        1.0f, -1.0f, -1.0f,  0.0f, 1.0f, 1.0f,
        1.0f, -1.0f,  1.0f,  0.0f, 1.0f, 1.0f,
        1.0f, -1.0f,  1.0f,  0.0f, 1.0f, 1.0f,
       -1.0f, -1.0f,  1.0f,  0.0f, 1.0f, 1.0f,
       -1.0f, -1.0f, -1.0f,  0.0f, 1.0f, 1.0f,

       // top face, blue
       -1.0f,  1.0f, -1.0f,  0.0f, 0.0f, 1.0f,
        1.0f,  1.0f, -1.0f,  0.0f, 0.0f, 1.0f,
        1.0f,  1.0f,  1.0f,  0.0f, 0.0f, 1.0f,
        1.0f,  1.0f,  1.0f,  0.0f, 0.0f, 1.0f,
       -1.0f,  1.0f,  1.0f,  0.0f, 0.0f, 1.0f,
       -1.0f,  1.0f, -1.0f,  0.0f, 0.0f, 1.0f,
    };

    // positions of sun, earth, moon
    glm::vec3 cubePositions[] = {
        glm::vec3(0.0f, 0.0f,  0.0f),
        glm::vec3(24.0f, 0.0f, 0.0f),
        glm::vec3(12.0f, 0.0f, 0.0f)
    };

    // angle of rotation of axis
    GLfloat cubeRotationAngle[] = {
        0.0f,
        -23.5f,
        0.0f
    };

    // direction that the object revolves
    glm::vec3 cubeRotationAxis[] = {
        glm::vec3(0.0f, 1.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 1.0f),
        glm::vec3(0.0f, 1.0f, 0.0f)
    };

    // direction that the object orbits
    glm::vec3 cubeRotationAxis2[] = {
        glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(0.0f, 1.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f)
    };
    
    // size of sun, earth, moon
    glm::vec3 cubeResize[] = {
        glm::vec3(6.0f, 6.0f, 6.0f),
        glm::vec3(3.0f, 3.0f, 3.0f),
        glm::vec3(1.5f, 1.5f, 1.5f)
    };

    // VBO & VAO 
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // day information
    float day = 0.0f;
    float inc = 1.0 / 24.0f;
  
    // render loop
    while (!glfwWindowShouldClose(window))
    {
        // input
        processInput(window);
        
        // render
        glClearColor(0.3f, 0.4f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // also clear the depth buffer now!

        // activate shader
        ourShader.use();

        // increment day
        day +=inc;

        // initialize matrices
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 projection = glm::mat4(1.0f);

        // keep track of earth and moon positions
        glm::vec3 earthPos = glm::vec3(1.0f);
        glm::vec3 moonPos = glm::vec3(1.0f);

        glBindVertexArray(VAO);

        // create projection matrix and send to shader
        projection = glm::perspective(glm::radians(30.0f), float(4.0 / 3.0), 0.1f, 1000.0f);
        ourShader.setMat4("projection", projection);

        // render 3 cubes
        for (unsigned int i = 0; i < 3; i++)
        {   
            // initialize model matrix
            glm::mat4 model = glm::mat4(1.0f);
            
            // if shape = earth or mooon
            if (i == 1 or i == 2) {
                // rotate around the sun
                glm::mat4 earthOrbit = glm::rotate(glm::mat4(1.0f), glm::radians(get_earth_rotate_angle_around_sun(day)), cubeRotationAxis2[1]);

                // translate to radius of orbit (x=24)
                glm::mat4 earthTrans = glm::translate(glm::mat4(1.0f), cubePositions[1]);

                // derotate
                glm::mat4 unearthOrbit = glm::rotate(glm::mat4(1.0f), glm::radians(-1.0f * get_earth_rotate_angle_around_sun(day)), cubeRotationAxis2[1]);
                
                // multiply to get model matrix
                model = earthOrbit * earthTrans * unearthOrbit * model;

                // update earth position
                earthPos = glm::vec3(model[3]);
             
            }

            // if shape = moon
            if (i == 2) {
                // rotate around the earth
                glm::mat4 moonOrbit = glm::rotate(glm::mat4(1.0f), glm::radians(get_moon_rotate_angle_around_earth(day)), cubeRotationAxis2[2]);

                // translate to radius of orbit (x=12)
                glm::mat4 moonTrans = glm::translate(glm::mat4(1.0f), cubePositions[2]);

                // derotate
                glm::mat4 unmoonOrbit = glm::rotate(glm::mat4(1.0f), glm::radians(-1.0f * get_moon_rotate_angle_around_earth(day)), cubeRotationAxis2[2]);

                // multiply to get model matrix
                model = moonOrbit * moonTrans * unmoonOrbit * model;

                // update moon position
                moonPos = glm::vec3(model[3]);
            }

            // scale cubes to sun, moon, earth size
            model = glm::scale(model, cubeResize[i]);

            // if shape = sun
            if (i == 0) {
                // rotate around self for revolution
                model = glm::rotate(model, glm::radians(get_sun_rotate_angle_around_itself(day)), cubeRotationAxis[0]);
            }

            // if shape = earth
            if (i == 1) {
                // rotate axis
                model = glm::rotate(model, glm::radians(cubeRotationAngle[1]), cubeRotationAxis[1]);

                // rotate around self for revolution
                model = glm::rotate(model, glm::radians(get_earth_rotate_angle_around_itself(day)), cubeRotationAxis2[1]);
            }
            
            // if shape = moon
            if (i == 2) {
                // rotate around self for revolution
                model = glm::rotate(model, glm::radians(get_moon_rotate_angle_around_itself(day)), cubeRotationAxis2[2]);
            }
          
            // send model matrix to shader
            ourShader.setMat4("model", model);

            // draw triangles
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // set view
        view = glm::lookAt(glm::vec3(100.0f, 50.0f, 100.0f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.0f, 1.0f, 0.0f));
        
        // send view matrix to shader
        ourShader.setMat4("view", view);

        // swap buffers and poll IO events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // de-allocate resources
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    // terminate 
    glfwTerminate();

    return 0;
}

static unsigned int ss_id = 0;
void dump_framebuffer_to_ppm(std::string prefix, unsigned int width, unsigned int height) {
    int pixelChannel = 3;
    int totalPixelSize = pixelChannel * width * height * sizeof(GLubyte);
    GLubyte* pixels = new GLubyte[totalPixelSize];
    glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, pixels);
    std::string file_name = prefix + std::to_string(ss_id) + ".ppm";
    std::ofstream fout(file_name);
    fout << "P3\n" << width << " " << height << "\n" << 255 << std::endl;
    for (size_t i = 0; i < height; i++)
    {
        for (size_t j = 0; j < width; j++)
        {
            size_t cur = pixelChannel * ((height - i - 1) * width + j);
            fout << (int)pixels[cur] << " " << (int)pixels[cur + 1] << " " << (int)pixels[cur + 2] << " ";
        }
        fout << std::endl;
    }
    ss_id++;
    delete[] pixels;
    fout.flush();
    fout.close();
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    //press p to capture screen
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
    {
        std::cout << "Capture Window " << ss_id << std::endl;
        int buffer_width, buffer_height;
        glfwGetFramebufferSize(window, &buffer_width, &buffer_height);
        dump_framebuffer_to_ppm("Assignment0-ss", buffer_width, buffer_height);
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
