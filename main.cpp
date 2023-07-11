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
#include <stb/stb_image.h>          //WICHTIG NICHT REIEHNFOLGE 'ÄNDERN
#include <iostream>
#include <vector>
#include <time.h>




void framebuffer_size_callback(GLFWwindow* window, int width, int height);
//void get_mouse(GLFWwindow* window, double xpos, double ypos);
void get_scroll(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
vector<modelData> getwürfel(int num, glm::vec4 wegpos, glm::vec3 rightvec, glm::vec3 upvec, glm::vec3 frontvec);

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
int spur = 1;


int main()
{
    //sachen
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "e", NULL, NULL);
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


    Shader donutShader("deps/include/shader/dings_shader_v3001.vs", "deps/include/shader/dings_shader_v3001_fragment.fs");
    Shader modelShader("deps/include/shader/vertexshader.vs", "deps/include/shader/fragmentshader.fs");
    Shader würfelshader("deps/include/shader/würfelvertexshader.vs", "deps/include/shader/würfelfragmentshader.fs");
    Shader lichtshader("deps/include/shader/lichtvertexshader.vs", "deps/include/shader/lichtfragmentshader.fs");
    srand(time(NULL));

    glm::vec3 ingsesamtpos = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 insgesamtrot = glm::vec3(0.0f, 0.0f, 0.0f);
    std::cout<<"lade modelle\n";
    Model autoModel(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), ("keinpfad"), ("dateien/wege/auto.obj"));
    Model donut(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), ("keinpfad"), ("dateien/wege/donut.obj"));
    Model model0(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), ("keinpfad"), ("keinpfad"));
    Model model1(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(60.0f, 0.0f, 0.0f), ("dateien/wege/animation/weg1anim.txt"), ("dateien/wege/weg1.obj"));
    Model model2(glm::vec3(0.0f, -17.1f, 0.0f), glm::vec3(59.621f, 0.0f, 2.01f), ("dateien/wege/animation/weg2anim.txt"), ("dateien/wege/weg2.obj"));
    Model model4(glm::vec3(0.0f, 17.1f, 0.0f), glm::vec3(59.621f, 0.0f, -2.01f), ("dateien/wege/animation/weg4anim.txt"), ("dateien/wege/weg4.obj"));
    Model model3(glm::vec3(0.0f, 0.0f, 24.5f), glm::vec3(60.0f, 6.279f, 0.0f), ("dateien/wege/animation/weg3anim.txt"), ("dateien/wege/weg3.obj"));
    Model model5(glm::vec3(0.0f, 0.0f, -24.5f), glm::vec3(60.0f, -6.279f, 0.0f), ("dateien/wege/animation/weg5anim.txt"), ("dateien/wege/weg5.obj"));
    //Model model4(glm::vec3(0.0f, 28.1f, 0.0f), glm::vec3(60.0f, 0.0f, -7.1f), ("dateien/wege/animation/weg1anim.txt"), ("dateien/wege/weg4.obj"));
    //Model model5(glm::vec3(0.0f, 00.0f, 26.3f), glm::vec3(60.0f, 1.7329f, 0.0f), ("dateien/wege/animation/weg1anim.txt"), ("dateien/wege/weg5.obj"));
    vector<Model> modelvec{ model1, model2, model4, model3, model5 /*model5 */ };

    glm::mat4 m1(1.0f);
    glm::mat4 m2(1.0f);
    glm::mat4 m3(1.0f);
    glm::mat4 m4(1.0f);
    glm::mat4 m5(1.0f);


    vector<glm::mat4>currentmatrices{m1,m2,m3,m4,m5};   //muss mind so vielwie len.currentwege haben
    vector<Model> currentwege{model0, model1, model1, model1/* model4*/};
    vector<Donut> donuts;
    Donut defaultdonut(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,0.0f,0.0f), glm::vec3(0.0f, 0.0f, 0.0f), 4, 0);
    for (int i = 0; i < 100; i++) {
        donuts.push_back(defaultdonut);
    }

    int wegeTime = 2;
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
    
    int würfelNum = 20;
    vector<vector<modelData>> würfeldaten;
    vector<vector<modelData>> würfeldaten2;
    vector<glm::vec3> positionen;


    glm::vec3 current_upV(0.0f,1.0f,0.0f);
    glm::vec3 current_cPos(0.0f,0.0f,0.0f);
    glm::mat4 autoAPos;
    glm::mat4 autoLPos;
    glm::mat4 autoRPos;
    glm::vec4 originA(0.0f,0.0f,0.0f,1.0f);
    glm::vec4 originL(0.0f, 0.0f, 0.0f, 1.0f);
    glm::vec4 originR(0.0f, 0.0f, 0.0f, 1.0f);
    glm::vec3 origincP(1.0f, 1.0f, 1.0f);
    int score = 0;
    int num = 0;
    for (int i = 1; i < currentwege.size(); i++) {
        würfeldaten.push_back(getwürfel(würfelNum, currentmatrices[i][3],glm::vec3(0.0f,0.0f,4.0f), glm::vec3(0.0f,1.0f,0.0f), glm::vec3(1.0f,0.0f,0.0f)));
        würfeldaten2.push_back(getwürfel(würfelNum, currentmatrices[i][3], -glm::vec3(0.0f, 0.0f, 4.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
    }

    while (!glfwWindowShouldClose(window))
    {

        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glm::mat4 globalrotmat(1.0f);
        globalrotmat = glm::rotate(globalrotmat, glm::radians(globalrot.x), glm::vec3(1.0f, 0.0f, 0.0f));
        globalrotmat = glm::rotate(globalrotmat, glm::radians(globalrot.y), glm::vec3(0.0f, 1.0f, 0.0f));
        globalrotmat = glm::rotate(globalrotmat, glm::radians(globalrot.z), glm::vec3(0.0f, 0.0f, 1.0f));

        glm::mat4 globalreverserotmat(1.0f);

        glm::mat4 crotmat(1.0f);

        glm::mat4 projection = glm::perspective(glm::radians(75.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f);

        processInput(window);

        // render
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

       


        //--model--
        modelShader.use();

        modelShader.setVec3("light.direction", glm::vec3(autoAPos[3]) -current_cPos);
        modelShader.setVec3("light.ambient", 0.4f, 0.2f, 0.7f);
        modelShader.setVec3("light.diffuse", 0.7f, 0.7f, 0.7f);
        modelShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);

        modelShader.setFloat("material.shininess", 16.0f);

        modelShader.setMat4("projection", projection);
        modelShader.setVec3("viewPos", camera.Position);



        if (currentAnimC == 100) {
            counter++;
            currentAnimC = 0;
            globalpos += currentwege[1].newPos;
            globalrot += currentwege[1].rotation;
            for (int e = 1; e < würfeldaten.size(); e++) {
                for (int j = 1; j < würfeldaten[e - 1].size(); j++) {
                    //würfeldaten[e - 1][j - 1].pos /* += currentwege[1].newPos */ -= glm::vec3(glm::vec4(currentwege[1].newPos, 1.0f)*globalreverserotmat);
                }
            }
            
            currentwege.erase(currentwege.begin() + 1);
            currentwege.push_back(modelvec[rand() % size(modelvec)]);
            
        }

        // globalrot = glm::vec3(0.0f, 0.0f, 0.0f);





        globalreverserotmat = glm::rotate(globalreverserotmat, glm::radians(-globalrot.z), glm::vec3(0.0f, 0.0f, 1.0f));
        globalreverserotmat = glm::rotate(globalreverserotmat, glm::radians(-globalrot.y), glm::vec3(0.0f, 1.0f, 0.0f));
        globalreverserotmat = glm::rotate(globalreverserotmat, glm::radians(-globalrot.x), glm::vec3(1.0f, 0.0f, 0.0f));



        crotmat = glm::rotate(crotmat, glm::radians(globalrot.z), glm::vec3(0.0f, 0.0f, 1.0f));
        crotmat = glm::rotate(crotmat, glm::radians(-globalrot.y), glm::vec3(0.0f, 1.0f, 0.0f));

        crotmat = glm::rotate(crotmat, glm::radians(globalrot.x), glm::vec3(1.0f, 0.0f, 0.0f));




        autoAPos = currentwege[1].aMatrices[currentAnimC];
        autoLPos = currentwege[1].lMatrices[currentAnimC];
        autoRPos = currentwege[1].rMatrices[currentAnimC];

        current_cPos = currentwege[1].aCposi[currentAnimC];
        current_upV = glm::vec3(currentwege[1].uMatrices[currentAnimC][3] - currentwege[1].aMatrices[currentAnimC][3]);
       // cout << current_upV[0]<<" "<<current_upV[1]<<" "<<current_upV[2] << "\n";

        if (currentAnimC) {
            glm::vec4 newP = autoAPos[3];
            glm::vec4 oldP = currentwege[1].aMatrices[currentAnimC - 1][3];
            glm::vec4 dist = newP - oldP;
            glm::vec4 dist_rotate = dist * globalreverserotmat;
            originA += dist_rotate;
            autoAPos[3] = originA;

            oldP = currentwege[1].lMatrices[currentAnimC - 1][3];
            newP = autoLPos[3];
            dist = newP - oldP;
            dist_rotate = dist * globalreverserotmat;
            originL += dist_rotate;
            autoLPos[3] = originL;


            oldP = currentwege[1].rMatrices[currentAnimC - 1][3];
            newP = autoRPos[3];
            dist = newP - oldP;
            dist_rotate = dist * globalreverserotmat;
            originR += dist_rotate;
            autoRPos[3] = originR;

            glm::vec3 newcP = current_cPos;
            glm::vec3 oldcP = currentwege[1].aCposi[currentAnimC - 1];
            glm::vec3 dist_c = newcP - oldcP;
            glm::vec4 dist_rotate_c = glm::vec4(dist_c, 1.0f) * globalreverserotmat;

            origincP += glm::vec3(dist_rotate_c);
            current_cPos = origincP;
        }
        
        else {
            würfeldaten.erase(würfeldaten.begin());
            würfeldaten2.erase(würfeldaten2.begin());
            glm::vec3 rotation = globalrot;
            glm::mat4 rotationmatrix;

            for (int i = 1; i < currentwege.size(); i++) {
                currentmatrices[i] = glm::mat4(1.0f);
                currentmatrices[i] = glm::rotate(currentmatrices[i], glm::radians(globalrot.x), glm::vec3(1.0f, 0.0f, 0.0f));
                currentmatrices[i] = glm::rotate(currentmatrices[i], glm::radians(globalrot.y), glm::vec3(0.0f, 1.0f, 0.0f));
                currentmatrices[i] = glm::rotate(currentmatrices[i], glm::radians(globalrot.z), glm::vec3(0.0f, 0.0f, 1.0f));


                for (int e = i; e > 0; e--) {

                    currentmatrices[i] = glm::translate(currentmatrices[i], currentwege[i - e].newPos);
                    currentmatrices[i] = glm::rotate(currentmatrices[i], glm::radians(currentwege[i - e].rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
                    currentmatrices[i] = glm::rotate(currentmatrices[i], glm::radians(currentwege[i - e].rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
                    currentmatrices[i] = glm::rotate(currentmatrices[i], glm::radians(currentwege[i - e].rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
                }

                rotation += currentwege[i].rotation;
                rotationmatrix = glm::rotate(rotationmatrix, glm::radians(-rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
                rotationmatrix = glm::rotate(rotationmatrix, glm::radians(-rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));

                rotationmatrix = glm::rotate(rotationmatrix, glm::radians(-rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));


                if (i == würfeldaten.size()+1) {

                    glm::vec3 Crightvec = (glm::vec3(glm::vec4(0.0f, 0.0f, 1.0f, 1.0f) * globalreverserotmat));// *rotationmatrix));
                    glm::vec3 Cupvec = (glm::vec3(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f) * globalreverserotmat));// *rotationmatrix));
                    glm::vec3 Cfrontvec = (glm::vec3(glm::vec4(-1.0f, 0.0f, 0.0f, 1.0f)* globalreverserotmat));// *rotationmatrix));
                    
                    würfeldaten.push_back(getwürfel(würfelNum, currentmatrices[i][3], Crightvec * glm::vec3(3.0f), Cupvec, Cfrontvec));
                    würfeldaten2.push_back(getwürfel(würfelNum, currentmatrices[i][3], -Crightvec * glm::vec3(3.0f), Cupvec, Cfrontvec));
                }
                else if (i) {
                    for (int würfel = 0; würfel < würfeldaten[i - 1].size(); würfel++) {
                        würfeldaten[i - 1][würfel].pos -= würfeldaten[i - 1][würfel].e - glm::vec3(currentmatrices[i][3]);
                        würfeldaten2[i - 1][würfel].pos -= würfeldaten2[i - 1][würfel].e - glm::vec3(currentmatrices[i][3]);
                    }
                }



                //cout << Crightvec[0] << " " << Crightvec[1] << " " << Crightvec[2] << " \n";
                 //cout << Cfrontvec[0] << " " << Cfrontvec[1] << " " << Cfrontvec[2] << " f\n";

            }
            originA = glm::vec4(glm::vec3(autoAPos[3]) - glm::vec3(0.0f, 0.0f, 0.0f),1.0f) * globalrotmat;// *globalreverserotmat;// *globalreverserotmat;
            originA = autoAPos[3] *globalreverserotmat;
            originR = autoRPos[3] *globalreverserotmat;
            originL = autoLPos[3] *globalreverserotmat;
            origincP = glm::vec3(glm::vec4(current_cPos, 1.0f) * globalreverserotmat);

            glm::vec4 newP = autoAPos[3];
            glm::vec4 oldP = currentwege[1].aMatrices[currentAnimC][3];
            glm::vec4 dist = newP - oldP;
            glm::vec4 dist_rotate = dist * globalreverserotmat;
            originA += dist_rotate;
            autoAPos[3] = originA;

            oldP = currentwege[1].lMatrices[currentAnimC][3];
            newP = autoLPos[3];
            dist = newP - oldP;
            dist_rotate = dist * globalreverserotmat;
            originL += dist_rotate;
            autoLPos[3] = originL;


            oldP = currentwege[1].rMatrices[currentAnimC][3];
            newP = autoRPos[3];
            dist = newP - oldP;
            dist_rotate = dist * globalreverserotmat;
            originR += dist_rotate;
            autoRPos[3] = originR;

            glm::vec3 newcP = current_cPos;
            glm::vec3 oldcP = currentwege[1].aCposi[currentAnimC];
            glm::vec3 dist_c = newcP - oldcP;
            glm::vec4 dist_rotate_c = glm::vec4(dist_c, 1.0f) * crotmat;

            origincP += glm::vec3(dist_rotate_c);
            current_cPos = origincP;

            glm::vec4 e = glm::vec4(-8.0f, 0.0f, 0.0f, 1.0f) * globalreverserotmat;
            glm::vec3 lichtpos = origincP + glm::vec3(e);
            lightPos = glm::vec3(lichtpos);

           
        }
        cout << würfeldaten.size() << "\n";

        glm::mat4 view = camera.GetViewMatrix(current_cPos, originA, glm::vec3(0.0f,1.0f,0.0f));
        //autoAPos += globalpos;


        vector<glm::mat4> autoPositions{autoLPos,autoAPos,autoRPos};

        donutShader.use();
        donutShader.setVec3("light.ambient", 1.0f, 1.0f, 1.0f);
        donutShader.setVec3("light.diffuse", 0.7f, 0.7f, 0.7f);
        donutShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
        donutShader.setMat4("projection", projection);
        donutShader.setMat4("view", view);

        for (int i = 0; i < 100; i++) {
            if (donuts[i].spur < 4) {
                if (donuts[i].startframe == currentAnimC) {
                    if (donuts[i].spur == spur) {
                        score++;
                        donuts[i] = defaultdonut;
                    }
                    else {
                        donuts[i] = defaultdonut;
                    }
                }
                else {
                    glm::mat4 emodel(1.0f);
                    if (currentAnimC > donuts[i].startframe) {
                        num = (currentAnimC - donuts[i].startframe);

                        donuts[i].currentposition = glm::vec3(donuts[i].startpos);



                        donuts[i].currentposition += glm::vec3((donuts[i].destpos - donuts[i].startpos) / glm::vec3(100.0f, 100.0f, 100.0f) * glm::vec3(num, num, num));

                        glm::vec3 rotv = currentwege[1].rotation;
                        glm::mat4 rot(1.0f);
                        rot = glm::rotate(rot, glm::radians(-rotv.z), glm::vec3(0.0f, 0.0f, 1.0f));
                        rot = glm::rotate(rot, glm::radians(-rotv.y), glm::vec3(0.0f, 1.0f, 0.0f));
                        rot = glm::rotate(rot, glm::radians(-rotv.x), glm::vec3(1.0f, 0.0f, 0.0f));

                        donuts[i].currentposition = glm::vec3(glm::vec4(donuts[i].currentposition, 1.0f) * rot);

                        donuts[i].currentposition += currentwege[1].newPos;
                        donuts[i].currentposition = glm::vec3(glm::vec4(donuts[i].currentposition, 1.0f) * globalreverserotmat);


                        emodel = glm::translate(emodel, donuts[i].currentposition);

                    }
                    else {
                        num = 100 - (donuts[i].startframe - currentAnimC);

                        donuts[i].currentposition = glm::vec3(donuts[i].startpos);
                        donuts[i].currentposition += glm::vec3((donuts[i].destpos - donuts[i].startpos) / glm::vec3(100.0f, 100.0f, 100.0f) * glm::vec3(num, num, num));
                        donuts[i].currentposition = glm::vec3(glm::vec4(donuts[i].currentposition, 1.0f) * globalreverserotmat);
                        emodel = glm::translate(emodel, donuts[i].currentposition);
                    }
                    //cout << num << "\n";
                    //num = 100 - num;
                    //out << num << "nu\n";

                    //cout << score << "\n";
                    emodel = glm::scale(emodel, glm::vec3(20.0f, 20.0f, 20.0f));
                    emodel = glm::rotate(emodel, glm::radians(globalrot.x), glm::vec3(1.0f, 0.0f, 0.0f));

                    emodel = glm::rotate(emodel, glm::radians(globalrot.y), glm::vec3(0.0f, 1.0f, 0.0f));
                    emodel = glm::rotate(emodel, glm::radians(globalrot.z), glm::vec3(0.0f, 0.0f, 1.0f));

                    //cout << emodel[3][0] << " " << emodel[3][1] << " " << emodel[3][2] << " e"<< i << " \n";
                    donutShader.setMat4("model", emodel);
                    donut.Draw(donutShader);
                }
            }
        }
        modelShader.use();
        cout << " \n";
        modelShader.setVec3("light.ambient", 0.4f, 0.2f, 0.7f);
        modelShader.setVec3("light.diffuse", 0.7f, 0.7f, 0.7f);
        modelShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);

        //donut gen
        if (rand() % 40 == 1) {
            int dspur = rand() % 3;

            vector<glm::mat4> base = currentwege[2].matrices[dspur];
            glm::vec3 basepos = glm::vec3(base[currentAnimC][3]);

            glm::vec3 abstand = glm::vec3(glm::normalize(currentwege[2].uMatrices[currentAnimC][3] - currentwege[2].aMatrices[currentAnimC][3]) * glm::vec4(10.0f, 10.0f, 10.0f, 1.0f));
            
            //abstand += glm::vec3(0.0f, 0.0f, 0.0f);

            glm::vec3 posi = basepos + abstand;
            glm::vec3 fallD = abstand / glm::vec3(100.0f,100.0f,100.0f);
            donuts[currentAnimC] = Donut(posi, basepos, fallD, dspur, currentAnimC);
        }
        currentAnimC++;
        glm::mat4 model = autoPositions[spur];
        //model = glm::rotate(model, glm::radians(-globalrot.x), glm::vec3(1.0f, 0.0f, 0.0f));
        //model = glm::rotate(model, glm::radians(-globalrot.y), glm::vec3(0.0f, 1.0f, 0.0f));
        //model = glm::rotate(model, glm::radians(-globalrot.z), glm::vec3(0.0f, 0.0f, 1.0f));
        modelShader.setMat4("view", view);

        
        for (int i = 1; i < currentwege.size(); i++) {
            Model testmodel = currentwege[i];

            glm::mat4 model = currentmatrices[i];
            modelShader.setMat4("model", model);
            testmodel.Draw(modelShader);
        }
        modelShader.setMat4("model", model);
        autoModel.Draw(modelShader);

        //--würfel--
        würfelshader.use();
        würfelshader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
        würfelshader.setVec3("lightPos", lightPos);
        würfelshader.setVec3("viewPos", camera.Position);
        würfelshader.setMat4("projection", projection);
        würfelshader.setMat4("view", view);

        glm::vec4 transcoords = glm::vec4((currentwege[1].newPos.x, currentwege[1].newPos.y, currentwege[1].newPos.z, 1.0f));
        glm::mat4 transmate = globalrotmat;
        transmate = glm::translate(globalrotmat, currentwege[1].newPos);
        glm::vec4 transecoords = transmate * transcoords;   //??


        //draw würfel
        for (int e = 0; e < würfeldaten.size(); e++) {
            vector<modelData> cdatern = würfeldaten[e];
            //cout << cdatern[0].pos[0] << " \n";
            for (int i = 0; i < cdatern.size(); i++) {

                würfelshader.setVec3("objectColor", cdatern[i].color);

                glm::mat4 würfelmodel = glm::mat4(1.0f);
                würfelmodel = glm::translate(würfelmodel, cdatern[i].pos);
                würfelmodel = glm::scale(würfelmodel, glm::vec3(cdatern[i].scale));
                würfelmodel = glm::rotate(würfelmodel, glm::radians(float(glfwGetTime() * cdatern[i].rotSpeed)), glm::vec3(1.0f, 1.0f, 1.0f));
                würfelshader.setMat4("model", würfelmodel);

                glBindVertexArray(cubeVAO);
                glDrawArrays(GL_TRIANGLES, 0, 36);

            }
        }
        for (int e = 0; e < würfeldaten2.size(); e++) {
            vector<modelData> cdatern = würfeldaten2[e];
            for (int i = 0; i < cdatern.size(); i++) {

                würfelshader.setVec3("objectColor", cdatern[i].color);

                glm::mat4 würfelmodel = glm::mat4(1.0f);
                würfelmodel = glm::translate(würfelmodel, cdatern[i].pos);
                würfelmodel = glm::scale(würfelmodel, glm::vec3(cdatern[i].scale));
                würfelmodel = glm::rotate(würfelmodel, glm::radians(float(glfwGetTime() * cdatern[i].rotSpeed)), glm::vec3(1.0f, 1.0f, 1.0f));
                würfelshader.setMat4("model", würfelmodel);

                glBindVertexArray(cubeVAO);
                glDrawArrays(GL_TRIANGLES, 0, 36);

            }
        }

        //--lichtwürfel--
        lichtshader.use();
        lichtshader.setMat4("projection", projection);
        lichtshader.setMat4("view", view);

        glm::mat4 lichtmodel(1.0f);
        lichtmodel = glm::mat4(1.0f);
        lichtmodel = glm::translate(lichtmodel, lightPos);
        lichtmodel = glm::scale(lichtmodel, glm::vec3(2.2f));
        lichtmodel = glm::rotate(lichtmodel, glm::radians(float(glfwGetTime() * 100)), glm::vec3(1.0f, 1.0f, 1.0f));
        lichtshader.setMat4("model", lichtmodel);

        glBindVertexArray(lightCubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
float last = 0.0;

vector<modelData> getwürfel(int num,glm::vec4 wegpos, glm::vec3 rightvec, glm::vec3 upvec, glm::vec3 frontvec) {
    vector<modelData> datae;
    for (int c = 0; c < num; c++) {
        glm::vec3 color((float)(rand()) / (float)(RAND_MAX), (float)(rand()) / (float)(RAND_MAX), (float)(rand()) / (float)(RAND_MAX));
        glm::vec3 pos_e(wegpos);
        pos_e += rightvec * glm::vec3(8.0f);
        pos_e += upvec * glm::vec3( (float)( rand() % 20 - 10));
        pos_e += frontvec * glm::vec3((float)(rand() % 40));
        pos_e += rightvec * glm::vec3((float)(rand() % 20)+3);
        //pos_e = glm::vec3(glm::vec4(pos_e, 1.0f) * rot);
        modelData modeldata(glm::vec3(wegpos), 100.0f, pos_e, 3.0f, color);
        datae.push_back(modeldata);
    }
    return datae;
}

void processInput(GLFWwindow* window)
{
    if (glfwGetTime()>last+0.1) {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, true);
        }


        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
                camera.ProcessKeyboard(FORWARD, deltaTime * 4);
            }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            camera.ProcessKeyboard(BACKWARD, deltaTime * 4);
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            camera.ProcessKeyboard(LEFT, deltaTime * 4);
            if (spur) {
                spur -= 1;
            }
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            camera.ProcessKeyboard(RIGHT, deltaTime * 4);
            if (spur < 2) {
                spur += 1;
            }
        }
        last = (float)glfwGetTime();
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