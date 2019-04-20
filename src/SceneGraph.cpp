#include "../include/SceneGraph.h"

SceneGraph::SceneGraph(){}

void SceneGraph::addModel(Model* model) {
    model->set_id(models.size());
    models.push_back(model);
    vector<int> vt;
    adj.push_back(vt);
}

int SceneGraph::get_speed() const {
    return speed;
}

vector<Model*> SceneGraph::get_models() const {
    return models;
}

Model* SceneGraph::get_model_id(int idx) const {
    return models[idx];
}

void SceneGraph::addEdge(int a,int b) {
    adj[a].push_back(b);
}

void SceneGraph::set_speed(int sp) {
    speed = sp;
}

void SceneGraph::dfs_update(int source,const glm::vec3& trn) {
    models[source]->translate(trn);
    for (auto e:adj[source]){
        dfs_update(e,trn);
    }
}

SceneGraph::~SceneGraph(){}