#include "raylib.h"
#include "raymath.h"
#include "entities.h"

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "Make the world Cardless");

    // Init Player
    Player player = {0};
    player.rect = (Rectangle){400 - 72 / 2, 300 - 92, 72, 92};
    player.speed = 0;
    player.canJump = false;
    player.sprite = InitSprite("../assets/player.png", 72, 8, 3);

    // Init Environment
    EnvItem envItems[] = {
        {{0, 0, 1000, 400}, 0, LIGHTGRAY},
        {{0, 300, 10000, 300}, 1, GREEN},
        {{300, 200, 50, 100}, 1, GRAY},
        {{630, 140, 50, 160}, 1, GRAY},
        {{1030, 140, 50, 160}, 1, GRAY},
        // {{300, 200, 400, 20}, 1, GRAY},
        // {{250, 300, 100, 20}, 1, GRAY},
        // {{650, 300, 100, 20}, 1, GRAY},
    };

    Enemy enemy = {
        .sprite = InitSprite("../assets/enemy.png", 72, 8, 3),
        .rect = {600 - 72 / 2, 300 - 92, 72, 92},
        .speed = 1,
        .direction = -1,
    };

    int envItemsLength = sizeof(envItems) / sizeof(envItems[0]);

    Camera2D camera = {0};
    camera.target = (Vector2){player.rect.x, player.rect.y};
    camera.offset = (Vector2){screenWidth / 2.0f, screenHeight / 2.0f};
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    SetTargetFPS(60);
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())
    {
        // Update
        //----------------------------------------------------------------------------------
        float deltaTime = GetFrameTime();

        UpdateEnemy(&enemy, envItems, envItemsLength, deltaTime);
        UpdatePlayer(&player, envItems, envItemsLength, deltaTime);

        UpdateCameraCenterInsideMap(&camera, &player, envItems, envItemsLength, deltaTime, screenWidth, screenHeight);

        if (IsKeyPressed(KEY_R))
        {
            camera.zoom = 1.0f;
            player.rect.x = 400;
            player.rect.y = 280;
        }

        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(LIGHTGRAY);

        BeginMode2D(camera);

        for (int i = 0; i < envItemsLength; i++)
            DrawRectangleRec(envItems[i].rect, envItems[i].color);

        DrawSprite(&(player.sprite), (Vector2){player.rect.x, player.rect.y});
        DrawSprite(&(enemy.sprite), (Vector2){enemy.rect.x, enemy.rect.y});

        EndMode2D();

        DrawFPS(10, 10);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow(); // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

void UpdateCameraCenterInsideMap(Camera2D *camera, Player *player, EnvItem *envItems, int envItemsLength, float delta, int width, int height)
{
    camera->target = (Vector2){player->rect.x, player->rect.y};
    camera->offset = (Vector2){width / 2.0f, height / 2.0f};
    float minX = 1000, minY = 1000, maxX = -1000, maxY = -1000;

    for (int i = 0; i < envItemsLength; i++)
    {
        EnvItem *ei = envItems + i;
        minX = fminf(ei->rect.x, minX);
        maxX = fmaxf(ei->rect.x + ei->rect.width, maxX);
        minY = fminf(ei->rect.y, minY);
        maxY = fmaxf(ei->rect.y + ei->rect.height, maxY);
    }

    Vector2 max = GetWorldToScreen2D((Vector2){maxX, maxY}, *camera);
    Vector2 min = GetWorldToScreen2D((Vector2){minX, minY}, *camera);

    if (max.x < width)
        camera->offset.x = width - (max.x - width / 2);
    if (max.y < height)
        camera->offset.y = height - (max.y - height / 2);
    if (min.x > 0)
        camera->offset.x = width / 2 - min.x;
    if (min.y > 0)
        camera->offset.y = height / 2 - min.y;
}
