#include "GameObject.hpp"

GameObject::GameObject()
{
    transform = { 0 };
}

void GameObject::Start() 
{

}

void GameObject::Update()
{

}

void GameObject::Draw()
{
    if(model != nullptr)
    {
        DrawModel(*model, transform.translation, 1.0f, WHITE);
    }
}

void GameObject::GOLoadModel(Model& model)
{
    this->model = &model;
}

GameObject::~GameObject() = default;