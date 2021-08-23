#ifndef GAMEOBJECT_H
#define GAMEOJBECT_H

#include<vector>
#include<raylib.h>
#include<ostream>
#include<string>

class GameObject
{
public:
    GameObject();
    GameObject(std::string name);
    virtual ~GameObject() = default;
    
    std::string name;
    Transform transform;
    
    virtual void Awake();
    virtual void Start();
    virtual void Update();
    virtual void Draw();
    virtual const char* ToString();
    void GOLoadModel(Model& model);
    void SetActive(bool active);
    void SetChild(int index, GameObject* object);
    void AddChild(GameObject* object);
    void RemoveChild(GameObject* object);
    int GetChildCount() {return children.size();}
    void SetParent(GameObject* object);
    GameObject* GetParent() {return parent;}
    GameObject* GetChild(int index);
    //Should be moved to draw component
    Color baseColor;
protected:
    GameObject* parent;

    // Should change to hashSet
    std::vector<GameObject*> children;
    
    //Should be moved to draw component;
    Model* model;
    bool active;
};

#endif