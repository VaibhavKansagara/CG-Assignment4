#ifndef SceneGraph_H_
#define SceneGraph_H_

#include <GLFW/glfw3.h>
#include <bits/stdc++.h>
#include "../include/Model.h"

class SceneGraph {
public:
    SceneGraph();

    ~SceneGraph();

    void addModel(Model* model);

    int get_speed() const;

    // directional edge: a -> b
    void addEdge(int a,int b);

    vector<Model*> get_models() const;

    Model* get_model_id(int idx) const;

    void set_speed(int sp);

    void dfs_update(int source,const glm::vec3& trn);
private:
    vector<Model*> models;

    // limit on number of models.
    vector<vector<int>> adj;

    int speed;
};

#endif