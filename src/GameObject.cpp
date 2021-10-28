#include "CameraController.hpp"
#include <algorithm>

GameObject::GameObject()
{
    name = typeid(this).name();
    tag = "Untagged";
    started = false;
    transform = { 0 };
    transform.scale = {1, 1, 1};
    baseColor = WHITE;
    parent = nullptr;
    // children
    model = nullptr;
    active = true;
    // gameObjects.push_back(this);
}

// GameObject::~GameObject()
// {
//     std::remove(gameObjects.begin(), gameObjects.end(), this);
// }

GameObject::GameObject(std::string name) : GameObject()
{
    this->name = name;
    // printf(("->" + name).c_str());
}

void GameObject::Start(GameObject* scene, GameState* gameState) 
{
    started = true;
    if(parent == nullptr)
    {
        scene->AddChild(*this);
        SetParent(*scene);
    }
    for(GameObject* child : children)
    {
        child->Start(scene, gameState);
    }
}

void GameObject::Awake(GameObject* scene, GameState* gameState)
{

}

void GameObject::Update(GameObject* scene, GameState* gameState)
{

}

void GameObject::Draw()
{
    if(!active)
    {
        return;
    }

    // FRUSTUM CULLING PLACEHOLDER FOR TESTING
    CameraController* cameraController = CameraController::main; // Last left off here, I have to move this out to main, because cycle dependancy when GameObject has reference to CameraController
    Camera camera = cameraController->GetCamera();
    float zpClip, znClip, xMinClip, xMaxClip, alpha, beta, omega, b, v1;

    // Vertical culling
    Vector3 vToTarget = cameraController->target->transform.translation - cameraController->transform.translation;
    alpha = asin((cameraController->transform.translation.y - cameraController->target->transform.translation.y)/vToTarget.Magnitude());
    beta = 180 - alpha/DEG2RAD - camera.fovy / 2;
    omega = 180 - beta - camera.fovy;

    zpClip = vToTarget.Magnitude() * sin(camera.fovy/2 * DEG2RAD) / sin(beta * DEG2RAD);
    znClip = vToTarget.Magnitude() * sin(camera.fovy/2 * DEG2RAD) / sin(omega * DEG2RAD);

    // Horizontal culling
    v1 = sin((90 - camera.fovy / 2)*DEG2RAD) * GetScreenHeight()/2 - sin((camera.fovy / 2)*DEG2RAD);
    xMinClip = vToTarget.Magnitude() * GetScreenWidth() / v1 / 2;


    if(model != nullptr 
    && 
    transform.translation.z < (cameraController->followPosition.z + zpClip + 1 /* padding */ ) &&
    transform.translation.z > (cameraController->followPosition.z - znClip - 1 /* padding */ ) && 
    transform.translation.x < (cameraController->followPosition.x + xMinClip) &&
    transform.translation.x > (cameraController->followPosition.x - xMinClip)
    )
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

void GameObject::AddChild(GameObject& object)
{
    if(std::find(children.begin(), children.end(), &object) == children.end())
    {
        // printf("Adding child \n");
        children.push_back(&object);
        object.SetParent(*this);
    }
}
void GameObject::RemoveChild(GameObject& object)
{
    for(size_t i = 0; i < children.size(); i++)
    {
        if(children[i] == &object)
        {
            object.parent = nullptr;
            children[i] = children[children.size() - 1];
            children.pop_back();
            break;
        }
    }
}

GameObject* GameObject::GetChild(size_t index)
{
    if(index < children.size())
    {
        return children[index];
    }

    return nullptr;
}

GameObject* GameObject::GetChild(std::string name)
{
    for(uint32_t i = 0; i < children.size(); i++)
    {
        if(children[i]->name == name)
        {
            return children[i];
        }
    }

    return nullptr;
}

void GameObject::SetParent(GameObject& object)
{
    if(parent != nullptr && &object != parent)
    {
        // printf("Child already has parent \n");
        parent->RemoveChild(*this);
    }
    parent = &object;
    parent->AddChild(*this);
}

void GameObject::LoadGameObjectModel(Model& model)
{
    this->model = &model;
}

std::string GameObject::ToString()
{
    return name;
}

void GameObject::Destroy()
{
    active = false;
    LogCustom(0, "Destroying!");
    GameObject* parent = GetParent();
    if(parent != nullptr)
    {
        parent->RemoveChild(*this);
    }
    // for(auto child : children)
    // {
    //     child->Destroy();
    // }
    while(children.size() != 0)
    {
        GameObject* object = children.back();
        object->Destroy();
        children.pop_back();
    }

    delete this;
}
