#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <memory>

#include "scene/Model.h"

// should this be static or what ?? 
class Scene {
public:

    Scene() = default;

    // better to store them as the unique pointers
    std::vector<std::unique_ptr<Model>>& getModels();
    Model* getModel(int index);
    void addModel(std::unique_ptr<Model> model);
    void removeModel(int index);

    // update all models in the scene
    void onUpdate(float deltaTime);

private:
    std::vector<std::unique_ptr<Model>> m_models;
};

#endif // SCENE_H