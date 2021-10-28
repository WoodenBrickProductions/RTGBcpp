#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "GameController.hpp"
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
    
    static std::vector<GameObject*> gameObjects;

    std::string name;
    std::string tag;
    Transform transform;
    Color baseColor;
    bool started;
    bool active;

    virtual void Awake(GameObject* scene, GameState* gameState);
    virtual void Start(GameObject* scene, GameState* gameState);
    virtual void Update(GameObject* scene, GameState* gameState);
    virtual void Draw();
    virtual void Destroy();
    virtual std::string ToString();

    void LoadGameObjectModel(Model& model);
    void SetChild(int index, GameObject& object);
    void AddChild(GameObject& object);
    void RemoveChild(GameObject& object);
    int GetChildCount() {return children.size();}
    void SetParent(GameObject& object);
    GameObject* GetParent() {return parent;}
    GameObject* GetChild(size_t index);
    GameObject* GetChild(std::string name);

    //Should be moved to draw component
protected:
    GameObject* parent;

    // Should change to hashSet
    std::vector<GameObject*> children;
    
    //Should be moved to draw component;
    Model* model;
};

void Instantiate(GameObject& gameObject);
void Destroy(GameObject& gameObject);


#endif