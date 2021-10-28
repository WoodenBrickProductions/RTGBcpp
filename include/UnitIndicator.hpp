#ifndef UNITINDICATOR_HPP
#define UNITINDICATOR_HPP

#include "GameObject.hpp"

class UnitController;
class UnitIndicator : public GameObject
{
public:
    const float maxSize = 0.30f;
    UnitIndicator();
    virtual ~UnitIndicator() = default;
    virtual void Start(GameObject* scene, GameState* gameState) override;
    virtual void Update(GameObject* scene, GameState* gameState) override;
    virtual void Draw();

    void LoadTextureUnIn(Texture2D* texture);

protected:
    Texture2D* texture;
    UnitController* unit;
};


#endif