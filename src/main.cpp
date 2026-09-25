#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/ext/vector_float2.hpp>
#include <glm/gtc/random.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stb_image.h>
#include <Shader.hpp>

const int window_Width = 800;
const int window_Height = 600;
int direction = 30;
float speed = 0.01;

glm::vec2 pos = {0.1,0.0};
glm::vec4 theColor = {1.0,1.0,1.0,1.0};

GLuint genTexture(const char *path,int textureWrapS, int textureWrapT, int textureMinFilter, int textureMagFilter, bool genMipmap, int colormode,bool flipY){
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, textureWrapS);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, textureWrapT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, textureMinFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, textureMagFilter);
    stbi_set_flip_vertically_on_load(flipY);

    int width, height, nrChannels;
    unsigned char *data = stbi_load(path, &width, &height, &nrChannels, 0);

    if(data){
        glTexImage2D(GL_TEXTURE_2D, 0, colormode, width, height, 0, colormode, GL_UNSIGNED_BYTE, data);
        std::cout << "Texture Loaded successfully" << std::endl;
        if(genMipmap){
            glGenerateMipmap(GL_TEXTURE_2D);
            std::cout << "Mipmap generated successfully" << std::endl;
        }

    }
    else {
        std::cout << "Failed to load texture" << std::endl;
    }



    return texture;
    stbi_image_free(data);
}


GLFWwindow* createWindow(int width, int height)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(width, height, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
    }
    glfwMakeContextCurrent(window);

    return window;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window, true);}
}

int main(){

    GLFWwindow* window = createWindow(window_Width, window_Height);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }


    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    float vertices[] = {
        //pos                // color            // texture coords
         0.3f,  0.2f, 0.0f,  1.0f, 0.0f, 0.0f,   1.0f, 1.0f,
         0.3f, -0.2f, 0.0f,  0.0f, 1.0f, 0.0f,   1.0f, 0.0f,
        -0.3f, -0.2f, 0.0f,  0.0f, 0.0f, 1.0f,   0.0f, 0.0f,
        -0.3f,  0.2f, 0.0f,  1.0f, 1.0f, 0.0f,   0.0f, 1.0f
    };
    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 3,   // first triangle
        1, 2, 3    // second triangle
    };


    //shader
    Shader testShader(RESOURCE_DIR "shaders/vShader.vs",RESOURCE_DIR "shaders/fShader.fs");

    unsigned int VBO, VAO, EBO;
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    //Texture Load
    GLuint texture1 = genTexture(RESOURCE_DIR "Textures/Dvd.png",GL_CLAMP_TO_BORDER,GL_CLAMP_TO_BORDER,GL_NEAREST_MIPMAP_LINEAR,GL_LINEAR,1,GL_RGBA,1);

    testShader.use();
    testShader.setInt("texture1", 0);



    //main loop
    while(!glfwWindowShouldClose(window))
    {
        processInput(window);

        //--------------------
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);


        glm::mat4 trans = glm::mat4(1.0f);
        trans = glm::translate(trans, glm::vec3(pos.x, pos.y, 0.0f));
        trans = glm::scale(trans, glm::vec3(0.5f, 0.5f, 0.5f));


        testShader.use();
        testShader.setMatFloat4("transform", false, glm::value_ptr(trans));
        testShader.setFloat4v("theColor", glm::value_ptr(theColor));
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


        pos.x += cos(direction * (glm::pi<float>() / 180.0)) * speed;
        pos.y += sin(direction * (glm::pi<float>() / 180.0)) * speed;
        if(pos.x>=0.85){
            direction += (180 - (2*direction));
            pos.x = 0.85;
            theColor = {glm::linearRand(0.1, 1.0),glm::linearRand(0.1, 1.0),glm::linearRand(0.1, 1.0),1.0};
        } else if(pos.x<=-0.85) {
            direction += (180 - (2*direction));
            pos.x = -0.85;
            theColor = {glm::linearRand(0.1, 1.0),glm::linearRand(0.1, 1.0),glm::linearRand(0.1, 1.0),1.0};
        }
        if(pos.y>=0.9){
            direction -= (2*direction);
            pos.y = 0.9;
            theColor = {glm::linearRand(0.1, 1.0),glm::linearRand(0.1, 1.0),glm::linearRand(0.1, 1.0),1.0};
        } else if(pos.y<=-0.9) {
            direction -= (2*direction);
            pos.y = -0.9;
            theColor = {glm::linearRand(0.1, 1.0),glm::linearRand(0.1, 1.0),glm::linearRand(0.1, 1.0),1.0};
        }

        //--------------------

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    glfwTerminate();
    return 0;
}
