#include "entities.h"

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
}