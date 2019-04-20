#ifndef Controller_H_
#define Controller_H_

#include "View.h"
#include "Model.h"
#include "SceneGraph.h"

using namespace std;

extern const GLfloat screenwidth,screenheight;
extern glm::vec3 lightPos1;
extern glm::vec3 lightPos2;
extern glm::vec3 lightPos3;
extern glm::vec3 lightPos4;
extern int source1;
extern int source2;
extern int source3;
extern int source4;

class Controller{
public:
    Controller();

    ~Controller();

    void set_scene(const SceneGraph& scene);

    void add(Model* model);

    Point get_trans_coord(double x,double y);

    int find(double x,double y);

    void handleKeys(GLFWwindow* window, int key, int code, int action, int mods);

    void process_input(GLFWwindow* window);

private:
	SceneGraph scene;
	View* view;
};

#endif