#pragma warning(disable:4996)
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <shader/shader_m.h>
#include <sachen/camera.h>
#include <sachen/model.h>
#include <sachen/wegclass.h>
#include <sachen/filesystem.h>
#define STB_IMAGE_IMPLEMENTATION    //WICHTIG NCIHT WEG MCAHEN KRISSE VIERTEN GRADES
#include <stb/stb_image.h>          //WICHTIG NICHT REIEHNFOLGE 'ÄNDERN
#include <iostream>
#include <vector>


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void get_mouse(GLFWwindow* window, double xpos, double ypos);
void get_scroll(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main()
{
    //sachen
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, get_mouse);
    glfwSetScrollCallback(window, get_scroll);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    stbi_set_flip_vertically_on_load(true);

    glEnable(GL_DEPTH_TEST);
    Shader shader_e("deps/include/shader/vertexshader.vs", "deps/include/shader/fragmentshader.fs");


    Model model0(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), ("keinpfad"));
    Model model1(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(30.0f, 0.0f, 0.0f), ("dateien/wege/weg1.obj"));
    Model model2(glm::vec3(0.0f, -24.6f, 0.0f), glm::vec3(60.83f, 2.16f, 0.0f),("dateien/wege/weg2.obj"));
    vector<Model> modelvec;
    modelvec.push_back(model1);
    modelvec.push_back(model2);
    vector<Model> currentwege;
    currentwege.push_back(model0);
    currentwege.push_back(model1);
    currentwege.push_back(model2);
    currentwege.push_back(model2);
    //glEnable(GL_DEPTH_TEST);
    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        processInput(window);

        // render
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        shader_e.use();
        //lichteinstellungen
        shader_e.setVec3("light.direction", -1.0f, -0.5f, -0.0f);
        shader_e.setVec3("viewPos", camera.Position);

        shader_e.setVec3("light.ambient", 0.4f, 0.2f, 0.7f);
        shader_e.setVec3("light.diffuse", 0.7f, 0.7f, 0.7f);
        shader_e.setVec3("light.specular", 1.0f, 1.0f, 1.0f);

        shader_e.setFloat("material.shininess", 16.0f);

        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        shader_e.setMat4("projection", projection);
        shader_e.setMat4("view", view);

        for (int i = 1; i < currentwege.size(); i++){
            Model testmodel = currentwege[i];
            glm::mat4 model = glm::mat4(1.0f);
            shader_e.setMat4("model", model);
            model = glm::translate(model, currentwege[i-1].newPos);
            model = glm::rotate(model, glm::radians(currentwege[i - 1].rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
            model = glm::rotate(model, glm::radians(currentwege[i - 1].rotation.z), glm::vec3(0.0f, 1.0f, 0.0f));
            model = glm::rotate(model, glm::radians(currentwege[i - 1].rotation.y), glm::vec3(0.0f, 0.0f, 1.0f));
            model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	
            shader_e.setMat4("model", model);
            testmodel.Draw(shader_e); 
        }
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);


    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera.ProcessKeyboard(FORWARD, deltaTime * 3);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.ProcessKeyboard(BACKWARD, deltaTime * 3);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.ProcessKeyboard(LEFT, deltaTime * 3);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.ProcessKeyboard(RIGHT, deltaTime * 3);
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void get_scroll(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

void get_mouse(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}