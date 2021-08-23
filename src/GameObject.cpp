#include "GameObject.hpp"
#include <algorithm>


GameObject::GameObject()
{
    transform = { 0 };
    transform.scale = {1, 1, 1};
    baseColor = WHITE;
    model = nullptr;
    parent = nullptr;
    name = typeid(this).name();
    // printf(name.c_str());
}

GameObject::GameObject(std::string name) : GameObject()
{
    this->name = name;
    // printf(("->" + name).c_str());
}

void GameObject::Start() 
{
    // Assign default parent as Scene;
}

void GameObject::Awake()
{

}

void GameObject::Update()
{

}

void GameObject::Draw()
{
    if(model != nullptr)
    {
        DrawModelEx(*model, transform.translation, {}, 0, transform.scale, baseColor);
        DrawModelWiresEx(*model, transform.translation, {}, 0, transform.scale, BLACK);
    }
    if(!children.empty())
    {
        for(auto object = children.begin(); object != children.end(); ++object)
        {
            (*object)->Draw();
        }
    }
}

void GameObject::AddChild(GameObject* object)
{
    if(std::find(children.begin(), children.end(), object) == children.end())
    {
        // printf("Adding child \n");
        children.push_back(object);
    }
}
void GameObject::RemoveChild(GameObject* object)
{
    for(int i = 0; i < children.size(); i++)
    {
        if(children[i] == object)
        {
            children[i] = *children.end();
            children.pop_back();
            break;
        }
    }
}

GameObject* GameObject::GetChild(int index)
{
    if(index > -1 && index < children.size())
    {
        return children[index];
    }

    return nullptr;
}

void GameObject::SetParent(GameObject* object)
{
    void* ptr = &parent;
    // printf("%p \n", ptr);
    if(object == nullptr)
    {
        // Still don't know where to error handle, only scene can have no parent.
        return;
    }
    // printf("Is parent != nullptr %d", (parent != nullptr));
    // printf("Is object != parent %d \n", (object != parent));
    if(parent != nullptr && object != parent)
    {
        // printf("Child already has parent \n");
        parent->RemoveChild(this);
    }
    parent = object;
    parent->AddChild(this);
}

void GameObject::GOLoadModel(Model& model)
{
    this->model = &model;
}


void GameObject::SetActive(bool active)
{
    this->active = active;
}

const char* GameObject::ToString()
{
    return "GameObject";
}
