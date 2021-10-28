#include "UnitIndicator.hpp"
#include "CameraController.hpp"
#include "UnitController.hpp"

UnitIndicator::UnitIndicator()
{
    texture = nullptr;
    unit = nullptr;
}

void UnitIndicator::Start(GameObject* scene, GameState* gameState)
{
    GameObject::Start(scene, gameState);
    unit = dynamic_cast<UnitController*>(parent);
    if(unit == nullptr)
    {
        LogCustom(0, "UnitIndicator attached to invalid object");
        DestroyGameObjectDeffered(this);
        return;
    }
}
    
void UnitIndicator::Update(GameObject* scene, GameState* gameState) 
{

}

void UnitIndicator::Draw()
{
    if(texture != nullptr)
    {
        // Need to update raylib for this to work
        // DrawBillboard(CameraController::main->GetCamera(), *texture, transform.translation, 2.0f, WHITE);
    }
    Color color = WHITE;
    if(unit->unitStats.attackTime < 0)
    {
        color = WHITE;
    }
    else
    {
        color = RED;
    }
    float radius = maxSize * (1.0f - unit->unitStats.attackTime / unit->unitStats.attackSpeed);
    DrawSphere(parent->transform.translation + transform.translation, radius, color);
}

void UnitIndicator::LoadTextureUnIn(Texture2D* texture)
{
    this->texture = texture;
}
