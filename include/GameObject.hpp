#ifndef GAMEOBJECT_H
#define GAMEOJBECT_H

#include<raylib.h>

class GameObject
{
public:
    GameObject();
    Transform transform;
    virtual ~GameObject();
    void GOLoadModel(Model& model);
    virtual void Start();
    virtual void Update();
    virtual void Draw();
    
protected:
    GameObject* parent;
    GameObject* children;
    Model* model;
};

#endif