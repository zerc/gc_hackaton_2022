#include "raylib.h"

#ifndef GC_ENTITIES_H
#define GC_ENTITIES_H

#define G 1000
#define PLAYER_JUMP_SPD 650.0f
#define PLAYER_HOR_SPD 200.0f

typedef struct Player
{
    Vector2 position;
    float speed;
    bool canJump;
} Player;

typedef struct EnvItem
{
    Rectangle rect;
    int blocking;
    Color color;
} EnvItem;

//----------------------------------------------------------------------------------
// Module functions declaration
//----------------------------------------------------------------------------------
void UpdatePlayer(Player *player, EnvItem *envItems, int envItemsLength, float delta);
void UpdateCameraCenterInsideMap(Camera2D *camera, Player *player, EnvItem *envItems, int envItemsLength, float delta, int width, int height);

#endif // GC_ENTITIES_H