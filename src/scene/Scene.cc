#include "scene/Scene.h"

std::vector<std::unique_ptr<Model>>& Scene::getModels() {
    return m_models;
} 

Model* Scene::getModel(int index) {
    if(index < 0 || index >= m_models.size()) {
        return nullptr;
    }

    return m_models[index].get(); // returns raw pointer
}

void Scene::addModel(std::unique_ptr<Model> model) {
    m_models.push_back(std::move(model));
}

void Scene::removeModel(int index) {
    if(index < 0 || index >= m_models.size()) {
        return;
    }

    m_models.erase(m_models.begin() + index);
}

void Scene::onUpdate(float deltaTime) {
    // currently does nothing, but can be extended to update model animations, etc.
}
