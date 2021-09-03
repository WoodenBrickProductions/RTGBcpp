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
    std::string tag;
    Transform transform;
    Color baseColor;
    
    virtual void Awake();
    virtual void Start();
    virtual void Update();
    virtual void Draw();
    virtual std::string ToString();
    void GOLoadModel(Model& model);
    void SetActive(bool active);
    void SetChild(int index, GameObject& object);
    void AddChild(GameObject& object);
    void RemoveChild(GameObject& object);
    int GetChildCount() {return children.size();}
    void SetParent(GameObject& object);
    GameObject* GetParent() {return parent;}
    GameObject* GetChild(size_t index);
    //Should be moved to draw component
protected:
    GameObject* parent;

    // Should change to hashSet
    std::vector<GameObject*> children;
    
    //Should be moved to draw component;
    Model* model;
    bool active;
};

#endif