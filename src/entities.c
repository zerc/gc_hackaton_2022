#include "entities.h"

Sprite InitSprite(const char *fileName, int width, int framesSpeed, int framesTotal)
{
    Texture2D playerTexture = LoadTexture(fileName);

    Sprite sprite = {
        .texture = playerTexture,
        .frameRec = {0.0f, 0.0f, width, playerTexture.height},
        .width = width,
        .framesCounter = 0,
        .frameCurrent = 0,
        .framesSpeed = framesSpeed,
        .framesTotal = framesTotal,
    };

    return sprite;
}

void UpdateSprite(Sprite *sp, bool active)
{
    sp->framesCounter++;

    if (sp->framesCounter >= (60 / sp->framesSpeed))
    {
        sp->framesCounter = 0;

        if (active)
        {
            sp->frameCurrent++;

            if (sp->frameCurrent > sp->framesTotal)
                sp->frameCurrent = 0;

            sp->frameRec.x = sp->frameCurrent * sp->width;
        }
    }
}

void DrawSprite(Sprite *sprite, Vector2 target)
{
    DrawTextureRec(sprite->texture, sprite->frameRec, target, WHITE);
}

void UpdatePlayer(Player *player, EnvItem *envItems, int envItemsLength, float delta)
{
    if (IsKeyDown(KEY_LEFT))
        player->rect.x -= PLAYER_HOR_SPD * delta;
    if (IsKeyDown(KEY_RIGHT))
        player->rect.x += PLAYER_HOR_SPD * delta;
    if (IsKeyDown(KEY_SPACE) && player->canJump)
    {
        player->speed = -PLAYER_JUMP_SPD;
        player->canJump = false;
    }

    Rectangle *p = &(player->rect);

    int hitObstacle = 0;
    for (int i = 0; i < envItemsLength; i++)
    {
        EnvItem *ei = envItems + i;
        if (ei->blocking && CheckCollisionRecs(player->rect, ei->rect))
        {
            hitObstacle = 1;
            player->speed = 0.0f;
            Rectangle r = GetCollisionRec(player->rect, ei->rect);
            p->y = r.y - p->height;
        }
    }

    if (hitObstacle)
    {
        player->canJump = true;
    }
    else
    {
        player->rect.y += player->speed * delta;
        player->speed += G * delta;
        player->canJump = false;
    }

    UpdateSprite(&(player->sprite), IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_RIGHT));
}

void UpdateEnemy(Enemy *enemy, EnvItem *envItems, int envItemsLength, float delta)
{
    enemy->rect.x -= PLAYER_HOR_SPD * delta;

    Rectangle *p = &(enemy->rect);

    int hitObstacle = 0;
    for (int i = 0; i < envItemsLength; i++)
    {
        EnvItem *ei = envItems + i;
        if (ei->blocking && CheckCollisionRecs(enemy->rect, ei->rect))
        {
            hitObstacle = 1;
            enemy->speed = 0.0f;
            Rectangle r = GetCollisionRec(enemy->rect, ei->rect);
            p->y = r.y - p->height;
        }
    }

    if (hitObstacle)
    {
    }
    else
    {
        enemy->rect.y += enemy->speed * delta;
        enemy->speed += G * delta;
    }

    UpdateSprite(&(enemy->sprite), enemy->speed > 0);
}