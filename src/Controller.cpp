#include "../include/Controller.h"
#define NOT_ANY_MODEL -1

using namespace std;

extern vector<Texture> texture_list;

Controller::Controller(){}

void Controller::set_scene(const SceneGraph& scn) {
    scene = scn;
}

void Controller::add(Model* model){
    scene.addModel(model);
}

//because we have same view and projection matrix for all models. 
Point Controller::get_trans_coord(double x,double y){
    float winX,winY,winZ;
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    winX = (float)x;
    winY = (float)viewport[3] - (float)y;
    glReadPixels(x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);
	winX = (winX / 400) - 1;
	winY = (winY / 400) - 1;
	winZ = winZ * 2  - 1;
	return Point(winX,winY,winZ);
}

int Controller::find(double x,double y){
    Point trans_coord = get_trans_coord(x,y);
    int len = scene.get_models().size();
    for(int i=0;i<len;i++){
        glm::vec3 pos = glm::vec3(trans_coord.getX(),trans_coord.getY(),trans_coord.getZ());
        pos = glm::inverse(scene.get_model_id(i)->get_model()) * glm::vec4(pos,1.0f);
        Point temp(pos.x,pos.y,pos.z);
        if(scene.get_model_id(i)->is_inside(temp) == true){
            if(scene.get_model_id(i)->is_select() == false and 
                  scene.get_model_id(i)->get_is_select_rotate() == false){
                scene.get_model_id(i)->set_cursor_pos(glm::vec3(trans_coord.getX(),
                                                trans_coord.getY(),trans_coord.getZ()));
                scene.get_model_id(i)->set_selected(true);
            }
            return i;
        }
    }
    return -1;
}

void Controller::handleKeys(GLFWwindow* window, int key, int code, int action, int mods){
    if (key == GLFW_KEY_T  && action == GLFW_PRESS){
        unsigned int new_no = (no_t_press + 1) % 4;
        for (int i=0;i<scene.get_models().size();i++){
            scene.get_model_id(i)->set_texture(texture_list[new_no]);
        }
        no_t_press = new_no;
    }

    if(key == GLFW_KEY_M  && action == GLFW_PRESS){
        unsigned int new_no = (no_m_press + 1) % 3;
        for (int i=0;i<scene.get_models().size();i++){
            scene.get_model_id(i)->change_mapping();;
        }
        no_m_press = new_no;
    }

    double x,y;
    glfwGetCursorPos(window,&x,&y);
    int active_model = find(x,y);
    // cout << "active-model:" << active_model <<endl;

    if(key == GLFW_KEY_ESCAPE  && action == GLFW_PRESS){
        glfwSetWindowShouldClose(window,true);
    }

    if(key == GLFW_KEY_Q  && action == GLFW_PRESS){
        if (source1) source1 = 0;
        else source1 = 1;
    }

    if(key == GLFW_KEY_W  && action == GLFW_PRESS){
        if (source2) source2 = 0;
        else source2 = 1;   
    }

    if(key == GLFW_KEY_E  && action == GLFW_PRESS){
        if (source3) source3 = 0;
        else source3 = 1;
    }

    if(key == GLFW_KEY_R  && action == GLFW_PRESS){
        if (source4) source4 = 0;
        else source4 = 1;
    }

    if(active_model == NOT_ANY_MODEL) return;

    if(key == GLFW_KEY_KP_ADD  && action == GLFW_PRESS || key == GLFW_KEY_KP_ADD  && action == GLFW_REPEAT){
        Model* model = scene.get_model_id(active_model);
        model->scale(glm::vec3(1.05,1.05,1.05));
    }

    if(key == GLFW_KEY_KP_SUBTRACT  && action == GLFW_PRESS || key == GLFW_KEY_KP_SUBTRACT  && action == GLFW_REPEAT){
        Model* model = scene.get_model_id(active_model);;
        model->scale(glm::vec3(0.95,0.95,0.95));
    }
}

void Controller::process_input(GLFWwindow* window){
    double x,y;
    glfwGetCursorPos(window,&x,&y);
    int active_model = find(x,y);

    int len = scene.get_models().size();
    for(int i=0;i<len;i++){
        Model *model = scene.get_model_id(i);
        if (model->get_is_select_rotate()) {
            glm::vec3 centre;
            centre.x = (model->get_mini().getX() + model->get_maxi().getX()) / 2.0;
            centre.y = (model->get_mini().getY() + model->get_maxi().getY()) / 2.0;
            centre.z = (model->get_mini().getZ() + model->get_maxi().getZ()) / 2.0;
            model->set_angle(((int)model->get_angle() + 2) % 360);
            glm::quat quat = glm::angleAxis(glm::radians(model->get_angle()), glm::normalize(centre));
            model->set_rotate(glm::mat4_cast(quat) * model->get_rotate());
        }
    }

    if(active_model == NOT_ANY_MODEL) return;

    if(glfwGetMouseButton(window,GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS ||
            glfwGetMouseButton(window,GLFW_MOUSE_BUTTON_LEFT) == GLFW_REPEAT){
        float winX, winY,winZ;
        winX = x;
        winY = 800 - y;
        winX = (winX / 400) - 1;
        winY = (winY / 400) - 1;
        glReadPixels(x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);
        winZ = winZ*2-1;

        Model* model = scene.get_model_id(active_model);
        glm::vec3 oldPos = model->get_cursor_pos();
        glm::vec3 newPos = glm::vec3(winX,winY,winZ);
        glm::vec3 diff = newPos - oldPos;
        scene.dfs_update(model->get_id(),glm::vec3(diff.x,diff.y,0.0f));
        model->set_cursor_pos(newPos);
    }

    if(glfwGetMouseButton(window,GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE){
        Model* model = scene.get_model_id(active_model);
        model->set_selected(false);
    }

    if(glfwGetMouseButton(window,GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS ||
            glfwGetMouseButton(window,GLFW_MOUSE_BUTTON_RIGHT) == GLFW_REPEAT){
        Model* model = scene.get_model_id(active_model);
        if (model->get_is_select_rotate())
            model->set_select_rotate(false);
        else 
            model->set_select_rotate(true);
    }
}

Controller::~Controller(){}
