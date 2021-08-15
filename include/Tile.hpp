#ifndef TILE_HPP
#define TILE_HPP

class GameObject;
struct GridPosition {
    int x;
    int y;
};

class Tile 
{
public:
    Tile() {}
    Tile(int x, int y, GameObject* occupiedObject);
    Tile(GridPosition gridPosition, GameObject* occupiedObject);
    virtual ~Tile();
private:
    GridPosition gridPosition;
    GameObject* occupiedObject;
};


#endif /* TILE_H */