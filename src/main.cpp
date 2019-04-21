#include<bits/stdc++.h>

#ifndef GL_GLEXT_PROTOTYPES
#define GL_GLEXT_PROTOTYPES
#endif

#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glx.h>
#include <GL/glext.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../include/Parser.h"
#include "../include/Shader.h"
#include "../include/Model.h"
#include "../include/View.h"
#include "../include/Controller.h"
#include "../include/SceneGraph.h"

using namespace std;

const GLfloat screenwidth = 800.0;
const GLfloat screenheight = 800.0;
vector<Texture> texture_list;
unsigned int no_t_press = 0;
unsigned int no_m_press = 0;

int source1=1,source2=1,source3=1,source4=1;
Controller controller;

// light souce position
glm::vec3 lightPos1(-0.5f, 0.5f, 0.0f);
glm::vec3 lightPos2(-0.1f, 0.5f, 0.0f);
glm::vec3 lightPos3(0.4f, 0.5f, 0.0f);
glm::vec3 lightPos4(0.9f, 0.5f, 0.0f);

void framebuffer_size_callback(GLFWwindow* window,int width,int height){
    glViewport(0,0,width,height);
}

GLFWwindow * create_window(){
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    GLFWwindow * window = glfwCreateWindow(800,800,"LearnOpengl",NULL,NULL);
    if(window == NULL){
        std::cout<<"Error creating window";
        glfwTerminate();
        exit(0);
    }
    glfwMakeContextCurrent(window); //make the current context our window.
    glfwSetFramebufferSizeCallback(window,framebuffer_size_callback);
    return window;
}

Model addModel(string name){
    Parser parser(name);
    Model model = parser.parse_file();
    return model;
}

static void handleKeys(GLFWwindow* window, int key, int code, int action, int mode){
  controller.handleKeys(window,key,code,action,mode);
}


int main(){
    GLFWwindow * window = create_window();

    //initialise shaders.
    Shader ourshader("shaders/vs.txt","shaders/fs.txt");
    Shader lightingShader("shaders/light_shader_vs.txt", "shaders/light_shaderfs.txt");

    //Create texture objects.
    texture_list.push_back(Texture("images/wall.jpg"));
    texture_list.push_back(Texture("images/checkerboard.png"));
    texture_list.push_back(Texture("images/mars.jpg"));
    texture_list.push_back(Texture("images/selfie.jpg"));
    //

    //Create Model and add models.
    Model A = addModel("data/sphere.ply");
    A.set_texture(texture_list[0]);
    A.scale(glm::vec3(0.1,0.1,0.1));
    A.translate(glm::vec3(-0.5f, 0.1f, 0.0f));
    A.pass_info_shader();

    Model B = addModel("data/sphere.ply");
    B.set_texture(texture_list[0]);
    B.scale(glm::vec3(0.1,0.1,0.1));
    B.translate(glm::vec3(-0.1f, 0.1f, 0.0f));
    B.pass_info_shader();

    Model C = addModel("data/sphere.ply");
    C.set_texture(texture_list[0]);
    C.scale(glm::vec3(0.1,0.1,0.1));
    C.translate(glm::vec3(0.4f, 0.1f, 0.0f));
    C.pass_info_shader();

    Model D = addModel("data/sphere.ply");
    D.set_texture(texture_list[0]);
    D.scale(glm::vec3(0.1,0.1,0.1));
    D.translate(glm::vec3(0.9f, 0.1f, 0.0f));
    D.pass_info_shader();
    //

    glEnable(GL_DEPTH_TEST);

    //light part starts
    Model light_source1 = addModel("data/sphere.ply");
    light_source1.set_light_source(true);
    light_source1.set_lightpos(lightPos1);
    light_source1.translate(lightPos1);
    light_source1.set_light_no(1);
    light_source1.scale(glm::vec3(0.01,0.01,0.01));
    light_source1.pass_info_shader();

    Model light_source2 = addModel("data/sphere.ply");
    light_source2.set_light_source(true);
    light_source2.set_lightpos(lightPos2);
    light_source2.translate(lightPos2);
    light_source2.scale(glm::vec3(0.01,0.01,0.01));
    light_source2.set_light_no(2);
    light_source2.pass_info_shader();

    Model light_source3 = addModel("data/sphere.ply");
    light_source3.set_light_source(true);
    light_source3.set_lightpos(lightPos3);
    light_source3.translate(lightPos3);
    light_source3.scale(glm::vec3(0.01,0.01,0.01));
    light_source3.set_light_no(3);
    light_source3.pass_info_shader();

    Model light_source4 = addModel("data/sphere.ply");
    light_source4.set_light_source(true);
    light_source4.set_lightpos(lightPos4);
    light_source4.translate(lightPos4);
    light_source4.scale(glm::vec3(0.01,0.01,0.01));
    light_source4.set_light_no(4);
    light_source4.pass_info_shader();
    //light part ends.

    //create a scene graph.
    SceneGraph scene;
    scene.addModel(&A);
    scene.addModel(&B);
    scene.addModel(&C);
    scene.addModel(&D);
    scene.addModel(&light_source1);    
    scene.addModel(&light_source2);
    scene.addModel(&light_source3);
    scene.addModel(&light_source4);
    // add the connections.
    scene.addEdge(0,4);
    scene.addEdge(1,5);
    scene.addEdge(2,6);
    scene.addEdge(3,7);
    scene.addEdge(1,2);
    scene.addEdge(2,3);
    //
    
    //create a view object
    View view;

    controller.set_scene(scene);

    while(!glfwWindowShouldClose(window)){
        glfwPollEvents();
        glfwSetKeyCallback(window,handleKeys);
        controller.process_input(window);
        glClearColor(0.2f,0.3f,0.3f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ourshader.use();
        ourshader.setVec3("lightColor1", 1.0f, 1.0f, 1.0f);
        ourshader.setVec3("lightPos1", lightPos1.x ,lightPos1.y,lightPos1.z);
        ourshader.setInt("source1",source1);

        ourshader.setVec3("lightColor2", 0.0f, 0.0f, 1.0f);
        ourshader.setVec3("lightPos2", lightPos2.x ,lightPos2.y,lightPos2.z);
        ourshader.setInt("source2",source2);

        ourshader.setVec3("lightColor3", 0.0f, 1.0f, 0.0f);
        ourshader.setVec3("lightPos3", lightPos3.x ,lightPos3.y,lightPos3.z);
        ourshader.setInt("source3",source3);

        ourshader.setVec3("lightColor4", 1.0f, 0.0f, 0.0f);
        ourshader.setVec3("lightPos4", lightPos4.x ,lightPos4.y,lightPos4.z);
        ourshader.setInt("source4",source4);


        //display all the models.
        view.display(A,ourshader);
        view.display(B,ourshader);
        view.display(C,ourshader);
        view.display(D,ourshader);

        // use lighting shader and display the source of the light.
        lightingShader.use();
        view.display(light_source1,lightingShader);
        view.display(light_source2,lightingShader);
        view.display(light_source3,lightingShader);
        view.display(light_source4,lightingShader);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    //keep in mind that you need to delete VAO before EBO as VAO has a pointer or 
    //something else which connects to EBO.
    glfwTerminate();
    return 0;
}