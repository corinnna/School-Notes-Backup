#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"

#include <iostream>
#include <fstream>

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

// path to face objects
// std::string obj_path = "data/faces/28.obj";

// path to cube object
// std::string obj_path = "data/cube.obj";

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// screen settings 
const unsigned int SCR_WIDTH = 1024;
const unsigned int SCR_HEIGHT = 768;

int main()
{
    // glfw: initialize and configure
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

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

    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    // build and compile our shader zprogram
    Shader ourShader("vshader.vs", "fshader.fs");

    // paths
    std::string paths[35] = {"data/faces/0.obj", "data/faces/1.obj", "data/faces/2.obj", 
                             "data/faces/3.obj", "data/faces/4.obj", "data/faces/5.obj", "data/faces/6.obj",
                             "data/faces/7.obj", "data/faces/8.obj", "data/faces/9.obj", "data/faces/10.obj",
                             "data/faces/11.obj", "data/faces/12.obj", "data/faces/13.obj", "data/faces/14.obj",
                             "data/faces/15.obj", "data/faces/16.obj", "data/faces/17.obj", "data/faces/18.obj",
                             "data/faces/19.obj", "data/faces/20.obj", "data/faces/21.obj", "data/faces/22.obj",
                             "data/faces/23.obj", "data/faces/24.obj", "data/faces/25.obj", "data/faces/26.obj",
                             "data/faces/27.obj", "data/faces/28.obj", "data/faces/29.obj", "data/faces/30.obj",
                             "data/faces/31.obj", "data/faces/32.obj", "data/faces/33.obj", "data/faces/34.obj"};
    
    // base path
    std::string obj_path = "data/faces/base.obj";

    // load base object
    tinyobj::attrib_t base_obj;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;

    // tinyobj load obj
    std::string warn, err;
    bool bTriangulate = true;
    bool bSuc = tinyobj::LoadObj(&base_obj, &shapes, &materials, &warn, &err, obj_path.c_str(), nullptr, bTriangulate);

    // c_str -> null char after it
    if (!bSuc)
    {
        std::cout << "tinyobj error: " << err.c_str() << std::endl;
        return -1;
    }

    // weight file path
    std::ifstream file("data/weights/11.weights");

    // set result object as base object
    tinyobj::attrib_t result_obj = base_obj;

    // open weight file
    if (file.is_open()) {
        std::string line;

        // counter to iterate through each object/shape
        int i = 0;

        // initialize variable to store weight
        float currWeight;

        while (std::getline(file, line)) {
            // get current weight
            currWeight = std::stof(line.c_str());

            // load current object
            std::string obj_path = paths[i];
            tinyobj::attrib_t curr_obj;
            std::vector<tinyobj::shape_t> shapes1;
            std::vector<tinyobj::material_t> materials1;

            // tinyobj load obj
            std::string warn, err;
            bool bTriangulate = true;
            bool bSuc = tinyobj::LoadObj(&curr_obj, &shapes1, &materials1, &warn, &err, obj_path.c_str(), nullptr, bTriangulate);

            // c_str -> null char after it
            if (!bSuc)
            {
                std::cout << "tinyobj error: " << err.c_str() << std::endl;
                return -1;
            }

            // make sure result and current object have the same number of vertices
            assert(curr_obj.vertices.size() == result_obj.vertices.size());

            // add weighted object to the sum
            for (unsigned i = 0; i < curr_obj.vertices.size(); i++)
            {
                result_obj.vertices[i] += currWeight * (curr_obj.vertices[i] - base_obj.vertices[i]);
            }
            
            // next object
            i++;
        }
        file.close();
    }

    // initialize vbuffer and nbuffer
    std::vector<float> vbuffer, nbuffer;

    // loads vertices and normals into vectors
    for (const auto& shape : shapes) {
        for (auto face : shapes[0].mesh.indices)
        {
            int vid = face.vertex_index;
            int nid = face.normal_index;

            //fill in vertex buffer with vertex positions
            vbuffer.push_back(result_obj.vertices[vid * 3]);
            vbuffer.push_back(result_obj.vertices[vid * 3 + 1]);
            vbuffer.push_back(result_obj.vertices[vid * 3 + 2]);

            //fill in normal buffer with normal directions
            nbuffer.push_back(base_obj.normals[nid * 3]);
            nbuffer.push_back(base_obj.normals[nid * 3 + 1]);
            nbuffer.push_back(base_obj.normals[nid * 3 + 2]);
        }
    }

    // bind buffers to render later
    unsigned int vertexVBO, normalVBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &vertexVBO);
    glGenBuffers(1, &normalVBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, vertexVBO);
    glBufferData(GL_ARRAY_BUFFER, vbuffer.size() * sizeof(float), &vbuffer[0], GL_STATIC_DRAW); 
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, normalVBO);
    glBufferData(GL_ARRAY_BUFFER, nbuffer.size() * sizeof(float), &nbuffer[0], GL_STATIC_DRAW); 
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);


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

        // initialize model matrix and send to shader
        glm::mat4 model = glm::mat4(1.0f);
        ourShader.setMat4("model", model);

        // initialize projection matrix and send to shader
        glm::mat4 proj = glm::perspective(glm::radians(60.0f), 4.0f / 3.0f, 0.1f, 1000.0f);
        ourShader.setMat4("projection", proj);

        // cube obj view matrix
        // glm::mat4 view = glm::lookAt(glm::vec3(-2, 6, 5), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));

        // render as wireframe
        // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        // face obj view matrix
        glm::mat4 view = glm::lookAt(glm::vec3(20, 50, 200), glm::vec3(0, 90, 0), glm::vec3(0, 1, 0));

        // send view matrix to shader
        ourShader.setMat4("view", view);

        // render box
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, vbuffer.size() / 3);


        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &vertexVBO);
    glDeleteBuffers(1, &normalVBO);


    // glfw: terminate, clearing all previously allocated GLFW resources.
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

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void processInput(GLFWwindow* window)
{
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

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
