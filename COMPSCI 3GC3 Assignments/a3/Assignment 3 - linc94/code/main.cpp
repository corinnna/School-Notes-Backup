#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"

#define TINYOBJLOADER_IMPLEMENTATION 
#include "tiny_obj_loader.h"

#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define _USE_MATH_DEFINES
#include <math.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// settings
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
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "A2", NULL, NULL);
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

    // build and compile our shaders
    Shader ourShader("default.vert", "default.frag");

    // variable to hold number of objects in scene
    const int num_objects = 3;


    /* paths if only one object in scene
    std::string obj_paths[num_objects] = {"asset/bucket.obj"};
    std::string tex_paths[num_objects] = {"asset/bucket.jpg"};*/
    

    /* paths if two objects in scene
    std::string obj_paths[num_objects] = {"asset/floor.obj","asset/bucket.obj"};
    std::string tex_paths[num_objects] = {"asset/floor.jpeg","asset/bucket.jpg"};*/
    
    // paths if three objects in scene
    std::string obj_paths[num_objects] = {"asset/floor.obj","asset/bucket.obj", "asset/timmy.obj"};
    std::string tex_paths[num_objects] = {"asset/floor.jpeg","asset/bucket.jpg", "asset/timmy.png"};

    // arrays to hold attributes shape, and material of each object
    tinyobj::attrib_t t_attrib[num_objects];
    std::vector<tinyobj::shape_t> t_shapes[num_objects];
    std::vector<tinyobj::material_t> t_materials[num_objects];

    // arrays to hold vbuffer, nbuffer, and tbuffer of each object 
    std::vector<float> vbuffers[num_objects];
    std::vector<float> nbuffers[num_objects];
    std::vector<float> tbuffers[num_objects];

    // store textures in a list
    GLuint textures[num_objects];

    // arrays to hold vertexVBO, normalVBO, texVBO, and VAO of each object 
    unsigned int vertexVBOs[num_objects];
    unsigned int normalVBOs[num_objects];
    unsigned int VAOs[num_objects];
    unsigned int texVBOs[num_objects];

    // for each object
    for (int i = 0; i < num_objects; i++) {
        // get the correct object and texture path
        std::string obj_path = obj_paths[i];
        std::string tex_path = tex_paths[i];

        // tinyobj load obj
        std::string warn, err;
        bool bTriangulate = true;
        bool bSuc = tinyobj::LoadObj(&t_attrib[i], &t_shapes[i], &t_materials[i], &warn, &err, obj_path.c_str(), nullptr, bTriangulate);
        if (!bSuc)
        {
            std::cout << "tinyobj error: " << err.c_str() << std::endl;
            return -1;
        }

        // put object details in buffers
        for (auto id : t_shapes[i][0].mesh.indices)
        {
            int vid = id.vertex_index;
            int nid = id.normal_index;
            int tid = id.texcoord_index;

            // fill in vertex positions
            vbuffers[i].push_back(t_attrib[i].vertices[vid * 3]);
            vbuffers[i].push_back(t_attrib[i].vertices[vid * 3 + 1]);
            vbuffers[i].push_back(t_attrib[i].vertices[vid * 3 + 2]);

            // normal
            nbuffers[i].push_back(t_attrib[i].normals[nid * 3]);
            nbuffers[i].push_back(t_attrib[i].normals[nid * 3 + 1]);
            nbuffers[i].push_back(t_attrib[i].normals[nid * 3 + 2]);

            // tex coord
            tbuffers[i].push_back(t_attrib[i].texcoords[tid * 2]);
            tbuffers[i].push_back(t_attrib[i].texcoords[tid * 2 + 1]);
        }

        // buffer stuff per object
        glGenVertexArrays(1, &VAOs[i]);
        glGenBuffers(1, &vertexVBOs[i]);
        glGenBuffers(1, &normalVBOs[i]);
        glGenBuffers(1, &texVBOs[i]);

        glBindVertexArray(VAOs[i]);

        glBindBuffer(GL_ARRAY_BUFFER, vertexVBOs[i]);
        glBufferData(GL_ARRAY_BUFFER, vbuffers[i].size() * sizeof(float), &vbuffers[i][0], GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, normalVBOs[i]);
        glBufferData(GL_ARRAY_BUFFER, nbuffers[i].size() * sizeof(float), &nbuffers[i][0], GL_STATIC_DRAW);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, texVBOs[i]);
        glBufferData(GL_ARRAY_BUFFER, tbuffers[i].size() * sizeof(float), &tbuffers[i][0], GL_STATIC_DRAW);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(2);

        // generate textures (only needs to be done once)
        if (i == 0) {
            glGenTextures(num_objects, textures);
        }

        // bind textures
        glBindTexture(GL_TEXTURE_2D, textures[i]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        int width, height, nrChannels;
        stbi_set_flip_vertically_on_load(true);
        unsigned char* data = stbi_load(tex_paths[i].c_str(), &width, &height, &nrChannels, 0);

        if (data)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            std::cout << "Failed to load texture floor" << std::endl;
        }
        stbi_image_free(data);

        ourShader.use();
    }


    ourShader.use();
    
    // since lightPos, ambientColor, cutOff, and k values are the same for all objects, we can just send it to the shader once
    // send ambientColor to shader
    ourShader.setVec3("ambientColor", 0.2f, 0.2f, 0.2f);
  
    /* configuration 1
    ourShader.setFloat("kc", 1.0f);
    ourShader.setFloat("kl", 0.35 * 1e-4);
    ourShader.setFloat("kq", 0.44 * 1e-4);*/

    // configuration 2
    // send kc, kl, and kq values to shader
    ourShader.setFloat("kc", 1.0f);
    ourShader.setFloat("kl", 0.35 * 1e-4);
    ourShader.setFloat("kq", 0.44 * 1e-4);

    // send cutOff value to shader
    ourShader.setFloat("cutOff", glm::cos(M_PI / 6.0f));

    // set lightPos and send to shader
    // t1 pos glm::vec3 lightPos(-50.0f, 0.0f, 300.0f
    glm::vec3 lightPos(0.0f, 200.0f, 0.0f);
    ourShader.setVec3("lightPos", lightPos);

    // initialize theta
    float theta = 0;

    // render loop
    while (!glfwWindowShouldClose(window))
    {
        // input
        processInput(window);

        // render
        glClearColor(0.3f, 0.4f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // also clear the depth buffer now!

        // render texture 1 (floor)
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textures[0]);
        glBindVertexArray(VAOs[0]);
        glDrawArrays(GL_TRIANGLES, 0, vbuffers[0].size() / 3);

        // render texture 2 (bucket)
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textures[1]);
        glBindVertexArray(VAOs[1]);
        glDrawArrays(GL_TRIANGLES, 0, vbuffers[1].size() / 3);

        // render texture 3 (timmy)
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textures[2]);
        glBindVertexArray(VAOs[2]);
        glDrawArrays(GL_TRIANGLES, 0, vbuffers[2].size() / 3);

        // use shader
        ourShader.use();

        /* rotate light for task 2
        glm::vec3 lightPos(sin(theta) * 150, 100, cos(theta) * 150);

        theta += 0.005f;

        ourShader.setVec3("lightPos", lightPos);*/

        // define each light as a matrix so that we can apply the rotation function on it to rotate it by theta in the y-axis
        glm::mat4 light = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        light = glm::rotate(light, theta, glm::vec3(0.0f, 1.0f, 0.0f));
        
        // set direction of red spotlight and rotate it by theta
        glm::vec3 redDirection(50.0f, -200.0f, -50.0f);
        glm::vec4 redRotation = light * glm::vec4(redDirection, 1.0f);
   
        // send diffuseColor and direction to shader
        ourShader.setVec3("redLight.diffuseColor", 1.0f, 0.0f, 0.0f);
        ourShader.setVec3("redLight.direction", redRotation);
        
        // set direction of blue spotlight and rotate it by theta
        glm::vec3 blueDirection(-50.0f, -200.0f, -50.0f);
        glm::vec4 greenRotation = light * glm::vec4(blueDirection, 1.0f);

        // send diffuseColor and direction to shader
        ourShader.setVec3("greenLight.diffuseColor", 0.0f, 1.0f, 0.0f);
        ourShader.setVec3("greenLight.direction", greenRotation);

        // set direction of blue spotlight and rotate it by theta
        glm::vec3 greenDirection(0.0f, -200.0f, 50.0f);
        glm::vec4 blueRotation = light * glm::vec4(greenDirection, 1.0f);

        // send diffuseColor and direction to shader
        ourShader.setVec3("blueLight.diffuseColor", 0.0f, 0.0f, 1.0f);
        ourShader.setVec3("blueLight.direction", blueRotation);
        
        // increment theta
        theta += 0.005;

        // create transformations
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 projection = glm::mat4(1.0f);

        ourShader.setMat4("model", model);

        view = glm::lookAt(glm::vec3(50, 100, 200), glm::vec3(0, 80, 0), glm::vec3(0, 1, 0)); // Camera is at (0,0,10), in World Space
        ourShader.setMat4("view", view);

        projection = glm::perspective(glm::radians(60.0f), 4.0f / 3.0f, 0.1f, 1000.0f); // Camera is at (0,0,10), in World Space
        ourShader.setMat4("projection", projection);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    glDeleteVertexArrays(num_objects, VAOs);
    glDeleteBuffers(num_objects, vertexVBOs);
    glDeleteBuffers(num_objects, normalVBOs);
    glDeleteBuffers(num_objects, texVBOs);


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
        dump_framebuffer_to_ppm("timmy_point2", buffer_width, buffer_height);
    }
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}