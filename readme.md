# RTGB project reproduction in C/C++

Knowledge:
* Cpp structs and class objects are pretty much the same thing as any type is a value type unless specified otherwise.


TODO:

* ~~Create file hierarchy for headers, source code, resources.~~
* ~~Create c++ source and .h examples.~~
* ~~Finish implementing Tile and GameObject~~
* ~~Create Tile matrix (tilemap[][])~~
* ~~Add snapping movement to default cube in main.c~~

# Milestone 1 (2021 08 09 - 08 23)
## Implement RTGB functionality from Unity up to creation of AI
* GameManager
* BoardController
  * TileMap
    * Tile
* CameraController
* GameObject
  * TileObject
    * UnitController
      * PlayerController

Update isn't overriden.

# Deprecated
```
# Todo today
* Add enemy units
  * ~~AIStats~~
  * Add enemy spawning
  * Add enemy collisions
  * Add enemy movement 
  * Make enemies attackable
  * Add chasing to enemy units
  * Add attacking to enemy units
* Add fsm class
  * Friend to all state classes
  * Stores states for each entity (states implementations and fsm implementation inside controller)
  * Fsm stores blackboard, states access blackboard through fsm

# Todo
* Proper object draw culling
  * Find Center of object
  * Get ViewFrustum
  * Draw only if inside Frustum
* Move data externally so I don't have to rebuild project every time I change a parameter

# Done
* ~~Camera follow~~
* ~~Object Culling~~
* ~~Add object destruction~~
  * ~~Remove from parent~~
  * ~~Destroy children~~
```