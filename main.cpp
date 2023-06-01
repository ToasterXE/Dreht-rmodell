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
#include <sachen/genmodeldata.h>
#define STB_IMAGE_IMPLEMENTATION    //WICHTIG NCIHT WEG MCAHEN KRISE VIERTEN GRADES
#include <stb/stb_image.h>          //WICHTIG NICHT REIEHNFOLGE '�NDERN
#include <iostream>
#include <vector>
#include <time.h>


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
//void get_mouse(GLFWwindow* window, double xpos, double ypos);
void get_scroll(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

Camera camera(glm::vec3(0.0f, 10.0f, 0.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;
glm::vec3 lightPos(15.2f, 8.0f, 25.0f);

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
    //glfwSetCursorPosCallback(window, get_mouse);
    glfwSetScrollCallback(window, get_scroll);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    stbi_set_flip_vertically_on_load(true);
    glEnable(GL_DEPTH_TEST);

    Shader modelShader("deps/include/shader/vertexshader.vs", "deps/include/shader/fragmentshader.fs");
    Shader w�rfelshader("deps/include/shader/w�rfelvertexshader.vs", "deps/include/shader/w�rfelfragmentshader.fs");
    Shader lichtshader("deps/include/shader/lichtvertexshader.vs", "deps/include/shader/lichtfragmentshader.fs");
    srand(time(NULL));

    glm::vec3 ingsesamtpos = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 insgesamtrot = glm::vec3(0.0f, 0.0f, 0.0f);
    std::cout<<"lade modelle\n";
    Model autoModel(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), ("keinpfad"), ("dateien/wege/auto.obj"));
    Model model0(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), ("keinpfad"), ("keinpfad"));
    Model model1(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(60.0f, 0.0f, 0.0f), ("dateien/wege/animation/weg1anim.txt"), ("dateien/wege/weg1.obj"));
    Model model2(glm::vec3(0.0f, -17.1f, 0.0f), glm::vec3(59.621f, 0.0f, 2.0116f), ("dateien/wege/animation/weg2anim.txt"), ("dateien/wege/weg2.obj"));
    Model model3(glm::vec3(0.0f, -28.1f, 0.0f), glm::vec3(60.0f, 0.0f, 7.1f), ("dateien/wege/animation/weg1anim.txt"), ("dateien/wege/weg3.obj"));
    Model model4(glm::vec3(0.0f, 28.1f, 0.0f), glm::vec3(60.0f, 0.0f, -7.1f), ("dateien/wege/animation/weg1anim.txt"), ("dateien/wege/weg4.obj"));
    Model model5(glm::vec3(0.0f, 00.0f, 26.3f), glm::vec3(60.0f, 1.7329f, 0.0f), ("dateien/wege/animation/weg1anim.txt"), ("dateien/wege/weg5.obj"));
    vector<Model> modelvec{ model1, model2, model3, model4, model5 };

    vector<Model> currentwege{model0, model1, model2, model1, model4};

    int wegeTime = 5;
    float animTime = 0.1;
    int currentAnimC = 0;
    int counter = 0;
    glm::vec3 globalrot(0.0f, 0.0f, 0.0f);
    glm::vec3 globalpos(0.0f, 0.0f, 0.0f);

    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };

    unsigned int VBO, cubeVAO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(cubeVAO);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    unsigned int lightCubeVAO;
    glGenVertexArrays(1, &lightCubeVAO);
    glBindVertexArray(lightCubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // note that we update the lamp's position attribute's stride to reflect the updated buffer data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    int w�rfelNum = 20;
    vector<modelData> w�rfeldaten;
    vector<glm::vec3> positionen;

    for (int c = 0; c < w�rfelNum; c++) {
        glm::vec3 color((float)(rand()) / (float)(RAND_MAX), (float)(rand()) / (float)(RAND_MAX), (float)(rand()) / (float)(RAND_MAX));
        glm::vec3 pos(camera.Position.x + (float)(rand()%50), (float)(rand()%40-30), camera.Position.z + (float)(rand()%50 + 30));

        modelData modeldata(100.0f, pos, 3.0f, color);
        w�rfeldaten.push_back(modeldata);
    }

    glm::vec3 current_lookat;
    glm::vec3 current_cPos;
    glm::mat4 current_autoPos;

    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        processInput(window);

        // render
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //--w�rfel--
        w�rfelshader.use();
        w�rfelshader.use();
        w�rfelshader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
        w�rfelshader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
        w�rfelshader.setVec3("lightPos", lightPos);
        w�rfelshader.setVec3("viewPos", camera.Position);
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f);
        glm::mat4 ranmat(1.0f);
        glm::vec4 transcoords = glm::vec4((currentwege[1].newPos.x, currentwege[1].newPos.y, currentwege[1].newPos.z, 1.0f));
        ranmat *= glm::rotate(ranmat, glm::radians(globalrot.x), glm::vec3(1.0f, 0.0f, 0.0f));            
        ranmat = glm::rotate(ranmat, glm::radians(globalrot.y), glm::vec3(0.0f, 1.0f, 0.0f));
        ranmat = glm::rotate(ranmat, glm::radians(globalrot.z), glm::vec3(0.0f, 0.0f, 1.0f));
        ranmat = glm::translate(ranmat, currentwege[1].newPos);
        glm::vec4 transecoords = ranmat * transcoords;
        //glm::mat4 view = camera.GetViewMatrix(glm::vec3(currentwege[1].aMatrices[currentAnimC][3][0], currentwege[1].aMatrices[currentAnimC][3][1], currentwege[1].aMatrices[currentAnimC][3][2]));
        glm::mat4 view = camera.GetViewMatrix(currentwege[1].aCposi[currentAnimC], glm::vec3(currentwege[1].aMatrices[currentAnimC][3][0], currentwege[1].aMatrices[currentAnimC][3][1], currentwege[1].aMatrices[currentAnimC][3][2]));
        //std::cout << transecoords.x << "\n";
        w�rfelshader.setMat4("projection", projection);
        w�rfelshader.setMat4("view", view);

        // world transformation
        /*glm::mat4 w�rfelmodel = glm::mat4(1.0f);
        w�rfelmodel = glm::translate(w�rfelmodel, glm::vec3(10.0f,10.0f,10.0f));
        w�rfelmodel = glm::scale(w�rfelmodel, glm::vec3(2.0f, 2.0f, 2.0f));
        w�rfelshader.setMat4("model", w�rfelmodel);

        // render the cube
        glBindVertexArray(cubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);*/
        for (int i = 0; i < w�rfeldaten.size(); i++) {
            w�rfelshader.setVec3("objectColor", w�rfeldaten[i].color);
            glm::mat4 w�rfelmodel = glm::mat4(1.0f);
            w�rfelmodel = glm::translate(w�rfelmodel, w�rfeldaten[i].pos);
            w�rfelmodel = glm::scale(w�rfelmodel, glm::vec3(w�rfeldaten[i].scale));
            w�rfelmodel = glm::rotate(w�rfelmodel, glm::radians(float(glfwGetTime() * w�rfeldaten[i].rotSpeed)), glm::vec3(1.0f, 1.0f, 1.0f));
            w�rfelshader.setMat4("model", w�rfelmodel);
            //std::cout << w�rfelmodel[0][0] << "  " << w�rfelmodel[0][1] << "  " << w�rfelmodel[0][2] << "  " << w�rfelmodel[0][3] << "\n";
            // render the cube
            glBindVertexArray(cubeVAO);
            glDrawArrays(GL_TRIANGLES, 0, 36);

        }

        //--lichtw�rfel--
        lichtshader.use();
        lichtshader.setMat4("projection", projection);
        lichtshader.setMat4("view", view);
        glm::mat4 lichtmodel(1.0f);
        lichtmodel = glm::mat4(1.0f);
        lichtmodel = glm::translate(lichtmodel, lightPos);
        lichtmodel = glm::scale(lichtmodel, glm::vec3(2.2f));
        lichtmodel = glm::rotate(lichtmodel, glm::radians(float(glfwGetTime() * 100)), glm::vec3(1.0f,1.0f,1.0f));
        lichtshader.setMat4("model", lichtmodel);

        glBindVertexArray(lightCubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);


        //--model--
        modelShader.use();
        //lichteinstellungen
        modelShader.setVec3("light.direction", -1.0f, -0.5f, -0.0f);
        modelShader.setVec3("viewPos", camera.Position);

        modelShader.setVec3("light.ambient", 0.4f, 0.2f, 0.7f);
        modelShader.setVec3("light.diffuse", 0.7f, 0.7f, 0.7f);
        modelShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);

        modelShader.setFloat("material.shininess", 16.0f);

        //glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f);
        modelShader.setMat4("projection", projection);

        //glm::mat4 viewmat = glm::mat4(1.0f);
        //glm::vec3 viewcoords = glm::vec3(currentwege[1].newPos);
        //glm::mat4 view = camera.GetViewMatrix(currentwege[1].newPos);
        //glm::mat4 view = camera.GetViewMatrix();
        modelShader.setMat4("view", view);

        for (int i = 1; i < currentwege.size(); i++){
            Model testmodel = currentwege[i];
            glm::mat4 model = glm::mat4(1.0f);
            //model = glm::translate(model, globalpos);
            model = glm::rotate(model, glm::radians(globalrot.x), glm::vec3(1.0f, 0.0f, 0.0f));
            model = glm::rotate(model, glm::radians(globalrot.y), glm::vec3(0.0f, 1.0f, 0.0f));
            model = glm::rotate(model, glm::radians(globalrot.z), glm::vec3(0.0f, 0.0f, 1.0f));
            for (int e = i; e > 0; e--) {
                model = glm::translate(model, currentwege[i - e].newPos);
                model = glm::rotate(model, glm::radians(currentwege[i - e].rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
                model = glm::rotate(model, glm::radians(currentwege[i - e].rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
                model = glm::rotate(model, glm::radians(currentwege[i - e].rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

            }
            modelShader.setMat4("model", model);
            testmodel.Draw(modelShader); 
        }
        //glm::mat4 model = glm::mat4(1.0f);
        //model = glm::translate(model, glm::vec3(60.0f, 0.0f, 0.0f));
        //glm::mat4 model = glm::mat4(1.0f);
        currentAnimC++;
        if (currentAnimC == 100) {
            counter++;
            currentAnimC = 0;
            globalpos += currentwege[1].newPos;
            globalrot += currentwege[1].rotation;
            currentwege.erase(currentwege.begin() + 1);
            currentwege.push_back(modelvec[rand() % size(modelvec)]);
        }
        current_autoPos = currentwege[1].aMatrices[currentAnimC];
        glm::mat4 model = current_autoPos;
        //model = glm::translate(model, globalpos);
        model = glm::rotate(model, glm::radians(globalrot.x), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(globalrot.y), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(globalrot.z), glm::vec3(0.0f, 0.0f, 1.0f));
        //model[0][3] = currentwege[1].aMatrices[80][0][3];
        /*cout << view[0][0] << "  " << view[0][1] << "  " << view[0][2] << "  " << view[0][3] << "\n";
        cout << view[1][0] << "  " << view[1][1] << "  " << view[1][2] << "  " << view[1][3] << "\n";
        cout << view[2][0] << "  " << view[2][1] << "  " << view[2][2] << "  " << view[2][3] << "\n";
        cout << view[3][0] << "  " << view[3][1] << "  " << view[3][2] << "  " << view[3][3] << "\n";
        cout << "\n";
        */
        //cout << 2 + model[0][3]<<"\n";

        modelShader.setMat4("model", model);
        autoModel.Draw(modelShader);

        //std::cout << glfwGetTime() <<"\n";

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
        camera.ProcessKeyboard(FORWARD, deltaTime * 4);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.ProcessKeyboard(BACKWARD, deltaTime * 4);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.ProcessKeyboard(LEFT, deltaTime * 4);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.ProcessKeyboard(RIGHT, deltaTime * 4);
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

/*void get_mouse(GLFWwindow* window, double xposIn, double yposIn)
{
    int e = 1;
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
  
    }*/