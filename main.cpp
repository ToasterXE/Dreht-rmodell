#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <shader/shader_m.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>



void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "e", NULL, NULL);

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);



    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))    //initialisierung von glad (wichtig)
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    glm::mat4 view = glm::mat4(1.0f);
    // note that we're translating the scene in the reverse direction of where we want to move
    view = glm::translate(view, glm::vec3(0.0f, -3.0f, -3.0f));
    glm::mat4 projection;
    projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 250.0f);
    

    Shader ourShader("deps/include/shader/vertexshader.vs", "deps/include/shader/fragmentshader.fs");
    float vertices[] = {
        0.5f,  0.5f, 0.5f, 1.0f, 0.0f, 0.0f,//rechts oben
       -0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 0.0f, // links unten
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, //rechts unten
       -0.5f,  0.5f, 0.5f, 0.5f, 0.5f, 0.0f, //links oben
        0.5f,  0.5f, -0.5f,0.5f, 0.0f, 0.5f,
       -0.5f, -0.5f, -0.5f,0.0f, 0.5f, 0.5f,
        0.5f, -0.5f, -0.5f,0.0f, 0.0f, 0.0f,
       -0.5f,  0.5f, -0.5f,1.0f, 1.0f, 1.0f
    };

    unsigned int indices[] = {  // note that we start from 0!
        1, 6, 2,    //unten
        1, 6, 5,
        4, 5, 6,  // hinten
        4, 5, 7,
        0, 6, 2,    //rechts
        0, 6, 4,
        3, 4, 0,    //oben
        3, 4, 7,
        3, 5, 7,    //links
        3, 5, 1,
        0, 1, 2,  // vorne
        0, 1, 3,
    };

    unsigned int indicesv2[] = {
        0, 1, 2,  // vorne
        0, 1, 3,
    };

    unsigned int VBO, VAO, EBO, EBOv2;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glGenBuffers(1, &EBOv2);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOv2);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesv2), indicesv2, GL_STATIC_DRAW);
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    while (!glfwWindowShouldClose(window))
    {

        processInput(window);
        glEnable(GL_DEPTH_TEST);

        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        ourShader.use();
        ourShader.setInt("texture1", 0);
        //glUseProgram(shaderProgram);
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 projection = glm::mat4(1.0f);
        model = glm::rotate(model, (float)glfwGetTime() * glm::radians(100.0f), glm::vec3(0.0f, 0.2f, 0.2f));    //drehung
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
        projection = glm::perspective(glm::radians(40.0f), 1.0f, 1.0f, 250.0f);

        unsigned int modelLoc = glGetUniformLocation(ourShader.ID, "model");
        unsigned int viewLoc = glGetUniformLocation(ourShader.ID, "view");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
        
        ourShader.setMat4("projection", projection);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOv2);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesv2), indicesv2, GL_STATIC_DRAW);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)   //falls die fenstergröße geändert wird
{
    glViewport(0, 0, width, height);
}

