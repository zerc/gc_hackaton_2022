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

    const int unitHeight = 91;     // size of all untis
    const int ceilingHeight = 300; // size of a floor

    InitWindow(screenWidth, screenHeight, "Make the world Cardless");

    // Init Player
    Player player = {0};
    player.rect = (Rectangle){80 - 72 / 2, 300 - unitHeight, 72, unitHeight};
    player.speed = 0;
    player.canJump = false;
    player.sprite = InitSprite("../assets/player.png", 72, 8, 3);

    // Init Environment
    EnvItem envItems[] = {
        {{-600, 300, 2048, 300}, GREEN, WITH_ALL},

        {{120, 200, 40, 100}, GRAY, WITH_NONE}, // door
        {{120, -300, 40, 500}, RED, WITH_ALL},  // left wall
        {{160, 100, 600, 20}, BLUE, WITH_ALL},  // ground floor ceiling

        {{900, -300, 40, 600}, RED, WITH_ALL},  // right wall
        {{300, -100, 600, 20}, BLUE, WITH_ALL}, // 1st floor ceiling

        {{160, -300, 740, 20}, BLUE, WITH_ALL}, // 2nd floor ceiling

        {{-200, -200, 10, 500}, BLANK, WITH_ALL}, // left wall

        // Limit enemies movement
        {{160, 200, 10, 100}, BLANK, WITH_ENEMY_ONLY},  // ground floor
        {{700, 0, 10, 100}, BLANK, WITH_ENEMY_ONLY},    // 1st floor
        {{300, -200, 10, 100}, BLANK, WITH_ENEMY_ONLY}, // 2nd floor

    };
    int envItemsLength = sizeof(envItems) / sizeof(envItems[0]);

    Enemy enemies[] = {
        {
            .sprite = InitSprite("../assets/enemy.png", 72, 8, 3),
            .rect = {600 - 72 / 2, 300 - unitHeight, 72, unitHeight},
            .speed = 1,
            .direction = -1,
        },
        {
            .sprite = InitSprite("../assets/enemy.png", 72, 8, 3),
            .rect = {300 - 72, 100 - unitHeight, 72, unitHeight},
            .speed = 3,
            .direction = 1,
        },
        {
            .sprite = InitSprite("../assets/enemy.png", 72, 8, 3),
            .rect = {400 - 72, -100 - unitHeight, 72, unitHeight},
            .speed = 3,
            .direction = 1,
        },

    };
    int enemiesCount = sizeof(enemies) / sizeof(enemies[0]);

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

        for (int i = 0; i < enemiesCount; i++)
            UpdateEnemy(&enemies[i], envItems, envItemsLength, deltaTime);

        UpdatePlayer(&player, envItems, envItemsLength, deltaTime);
        UpdateGameCamera(&camera, &player, envItems, envItemsLength, deltaTime, screenWidth, screenHeight);

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

        for (int i = 0; i < enemiesCount; i++)
            DrawSprite(&(enemies[i].sprite), (Vector2){enemies[i].rect.x, enemies[i].rect.y});

        DrawSprite(&(player.sprite), (Vector2){player.rect.x, player.rect.y});

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