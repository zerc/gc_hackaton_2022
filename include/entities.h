#include "raylib.h"

#ifndef GC_ENTITIES_H
#define GC_ENTITIES_H

#define G 1000
#define PLAYER_JUMP_SPD 650.0f
#define PLAYER_HOR_SPD 200.0f

typedef struct Sprite
{
    Texture2D texture;
    Rectangle frameRec;
    int width;
    int framesCounter;
    int framesSpeed;
    int frameCurrent;
    int framesTotal;
} Sprite;

typedef struct Player
{
    Rectangle rect;
    float speed;
    bool canJump;
    Sprite sprite;
} Player;

typedef struct EnvItem
{
    Rectangle rect;
    int blocking;
    Color color;
} EnvItem;

Sprite InitSprite(const char *fileName, int width, int framesSpeed, int framesTotal);
void UpdateSprite(Sprite *sprite);
void DrawSprite(Sprite *sprite, Vector2 target);

void UpdatePlayer(Player *player, EnvItem *envItems, int envItemsLength, float delta);
void UpdateCameraCenterInsideMap(Camera2D *camera, Player *player, EnvItem *envItems, int envItemsLength, float delta, int width, int height);

#endif // GC_ENTITIES_H