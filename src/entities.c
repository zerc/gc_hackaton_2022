#include <stdio.h>

#include "entities.h"
#include "raymath.h"

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
    if (player->enemyCollisionFrames > 0 && player->enemyCollisionFrames > 20)
    {
        player->isAlive = false;
        return;
    }

    int direction = 0;

    if (IsKeyDown(KEY_LEFT))
        direction = -1;
    if (IsKeyDown(KEY_RIGHT))
        direction = 1;

    player->rect.x += direction * PLAYER_HOR_SPD * delta;

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
        if ((ei->collisionGroup == WITH_ALL || ei->collisionGroup == WITH_PLAYER_ONLY) && CheckCollisionRecs(player->rect, ei->rect))
        {
            hitObstacle = 1;
            player->speed = 0.0f;

            Rectangle r = GetCollisionRec(player->rect, ei->rect);

            if (r.width > r.height)
            {
                if (p->y < ei->rect.y)
                {
                    p->y = r.y - p->height;
                }
                else
                {
                    // Do not allow to jump over the ceiling
                    p->y += 1;
                }
            }
            // "frontal" collision - limit x
            else if (direction == 1)
            {
                p->x = r.x - p->width;
            }
            else if (direction == -1)
            {
                p->x = ei->rect.x + ei->rect.width;
            }
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

void UpdateEnemy(Enemy *enemy, EnvItem *envItems, Player *player, int envItemsLength, float delta)
{
    if (enemy->speed == 0)
        return;

    enemy->rect.x += enemy->direction * enemy->speed * delta;

    Rectangle *p = &(enemy->rect);

    for (int i = 0; i < envItemsLength; i++)
    {
        EnvItem *ei = envItems + i;
        if ((ei->collisionGroup == WITH_ALL || ei->collisionGroup == WITH_ENEMY_ONLY) && CheckCollisionRecs(enemy->rect, ei->rect))
        {
            Rectangle r = GetCollisionRec(enemy->rect, ei->rect);

            if (r.width == enemy->rect.width)
            {
                // ground collision - limit y
                p->y = r.y - p->height;
            }
            // "frontal" collision - limit x
            else if (enemy->direction == 1)
            {
                p->x = r.x - p->width;
                enemy->direction *= -1;
            }
            else if (enemy->direction == -1)
            {
                p->x = ei->rect.x + ei->rect.width;
                enemy->direction *= -1;
            }
        }
    }

    if (CheckCollisionRecs(enemy->rect, player->rect))
    {

        Rectangle r = GetCollisionRec(enemy->rect, player->rect);

        if (r.height == player->rect.height)
        {
            player->enemyCollisionFrames += 1;
            if (enemy->direction == 1)
            {
                p->x = r.x - p->width;
                // enemy->direction *= -1;
            }
            else if (enemy->direction == -1)
            {
                p->x = player->rect.x + player->rect.width;
                // enemy->direction *= -1;
            }
            enemy->speed = 0.1;
        }

        if (IsKeyDown(KEY_ENTER) && player->enemyCollisionFrames > 0 && player->enemyCollisionFrames < 20)
        {
            enemy->speed = 0; // the enemey killed
            player->enemyCollisionFrames = 0;
        }
    }

    UpdateSprite(&(enemy->sprite), enemy->speed > 0);
}

void UpdateGameCamera(Camera2D *camera, Player *player, EnvItem *envItems, int envItemsLength, float delta, int width, int height)
{
    static float minSpeed = 30;
    static float minEffectLength = 10;
    static float fractionSpeed = 0.8f;

    camera->offset = (Vector2){width / 2.0f, height / 2.0f};
    Vector2 diff = Vector2Subtract((Vector2){player->rect.x, player->rect.y}, camera->target);
    float length = Vector2Length(diff);

    if (length > minEffectLength)
    {
        float speed = fmaxf(fractionSpeed * length, minSpeed);
        camera->target = Vector2Add(camera->target, Vector2Scale(diff, speed * delta / length));
    }
};