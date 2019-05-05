#include "../include/SceneGraph.h"

SceneGraph::SceneGraph(){
    speed = 0;
    modelnum = 0;
}

void SceneGraph::addModel(Model* model) {
    model->set_id(models.size());
    models.push_back(model);
    vector<int> vt;
    adj.push_back(vt);
}

float SceneGraph::get_speed() const {
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
    models[a]->addChild(models[b]);
}

void SceneGraph::set_speed(float sp) {
    speed = sp;
}

void SceneGraph::dfs_update(int source,const glm::vec3& trn) {
    models[source]->translate(trn);
    for (auto e:adj[source]){
        dfs_update(e,trn);
    }
}

void SceneGraph::update(){
    models[0]->update(speed, glm::vec3(0.0, 0.0, 0.0), glm::mat4(1.0));
}

void SceneGraph::setMotion(int index,int m) {
    models[index]->setMotion(m);
}

SceneGraph::~SceneGraph(){}