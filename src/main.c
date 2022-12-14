#include "raylib.h"
#include "raymath.h"
#include "entities.h"
#include "textures.h"

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

    Texture2D background = LoadTexture("../assets/sky.png");
    Texture2D brick = LoadTexture("../assets/brick.png");
    Texture2D floorOne = LoadTexture("../assets/floor_1.png");
    Texture2D floorTwo = LoadTexture("../assets/floor_2.png");
    Texture2D floorThree = LoadTexture("../assets/floor_3.png");
    Texture2D grass = LoadTexture("../assets/grass.png");
    Texture2D tree = LoadTexture("../assets/tree.png");
    Texture2D temple = LoadTexture("../assets/temple.png");
    Texture2D win = LoadTexture("../assets/win.png");
    Texture2D lose = LoadTexture("../assets/lose.png");

    // Init Player
    Player player = {0};
    player.rect = (Rectangle){80 - 72 / 2, 300 - unitHeight, 72, unitHeight};
    player.speed = 0;
    player.canJump = false;
    player.sprite = InitSprite("../assets/player.png", 72, 8, 3);
    player.enemyCollisionFrames = 0; // no collision with an enemy
    player.isAlive = true;
    player.attackFrames = 10;
    player.attackFramesCurrent = 0;

    // Init Environment
    EnvItem envItems[] = {
        {{-600, 300, 2048, 300}, GREEN, &grass, WITH_ALL},
        {{120, 100, 780, 200}, GRAY, &floorOne, WITH_NONE},    // facade floor1
        {{120, -100, 780, 200}, GRAY, &floorTwo, WITH_NONE},   // facade floor2
        {{120, -300, 780, 200}, GRAY, &floorThree, WITH_NONE}, // facade floor3

        {{120, 200, 40, 100}, GRAY, 0, WITH_NONE},     // door
        {{120, -300, 40, 500}, RED, &brick, WITH_ALL}, // left wall
        {{160, 100, 600, 20}, BLUE, &brick, WITH_ALL}, // ground floor ceiling

        {{900, -300, 40, 600}, RED, &brick, WITH_ALL},  // right wall
        {{300, -100, 600, 20}, BLUE, &brick, WITH_ALL}, // 1st floor ceiling

        {{160, -300, 740, 20}, BLUE, &brick, WITH_ALL},     // 2nd floor ceiling
        {{360, -430, 223, 131}, BLANK, &temple, WITH_NONE}, // 2nd floor ceiling

        {{-200, 7, 136, 293}, BLANK, &tree, WITH_ALL}, // left screen limiter
        {{-200, -200, 10, 500}, BLANK, 0, WITH_ALL},   // left screen limiter

        // Limit enemies movement
        {{160, 200, 10, 100}, BLANK, 0, WITH_ENEMY_ONLY},  // ground floor
        {{700, 0, 10, 100}, BLANK, 0, WITH_ENEMY_ONLY},    // 1st floor
        {{300, -200, 10, 100}, BLANK, 0, WITH_ENEMY_ONLY}, // 2nd floor
        {{750, -200, 10, 100}, BLANK, 0, WITH_ENEMY_ONLY}, // 2nd floor

    };
    int envItemsLength = sizeof(envItems) / sizeof(envItems[0]);

    Enemy enemies[] = {
        {
            .sprite = InitSprite("../assets/enemy.png", 72, 8, 3),
            .rect = {600 - 72 / 2, 300 - unitHeight, 72, unitHeight},
            .speed = 250,
            .direction = -1,
            .isAlive = true,
            .isVisible = true,
        },
        {
            .sprite = InitSprite("../assets/enemy.png", 72, 8, 3),
            .rect = {300 - 72, 100 - unitHeight, 72, unitHeight},
            .speed = 300,
            .direction = 1,
            .isAlive = true,
            .isVisible = true,
        },
        {
            .sprite = InitSprite("../assets/enemy.png", 72, 8, 3),
            .rect = {400 - 72, -100 - unitHeight, 72, unitHeight},
            .speed = 350,
            .direction = 1,
            .isAlive = true,
            .isVisible = true,
        },

    };
    int enemiesCount = sizeof(enemies) / sizeof(enemies[0]);
    int activeEnemiesCount = 0;

    Camera2D camera = {0};
    camera.target = (Vector2){player.rect.x, player.rect.y};
    camera.offset = (Vector2){screenWidth / 2.0f, screenHeight / 2.0f};
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    // Variables for the final screens
    const int winBarMax = 200;
    int winBarCurrent = 0;

    SetTargetFPS(60);
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())
    {
        // Update
        //----------------------------------------------------------------------------------
        float deltaTime = GetFrameTime();

        UpdatePlayer(&player, envItems, envItemsLength, deltaTime);

        if (player.isAlive)
        {
            activeEnemiesCount = 0;

            for (int i = 0; i < enemiesCount; i++)
            {
                UpdateEnemy(&enemies[i], envItems, &player, envItemsLength, deltaTime);
                if (enemies[i].isVisible)
                    activeEnemiesCount += 1;
            }

            UpdateGameCamera(&camera, &player, envItems, envItemsLength, deltaTime, screenWidth, screenHeight);

            if (IsKeyPressed(KEY_R))
            {
                camera.zoom = 1.0f;
                player.rect.x = 400;
                player.rect.y = 280;
            }
        }

        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(WHITE);

        DrawTexture(background, 0, 0, WHITE);

        BeginMode2D(camera);

        for (int i = 0; i < envItemsLength; i++)
        {
            if (envItems[i].texture)
            {
                DrawTextureTiled(*(envItems[i].texture), (Rectangle){0, 0, envItems[i].texture->width, envItems[i].texture->height}, envItems[i].rect, (Vector2){0, 0}, 0, 1, WHITE);
            }
            else
            {
                DrawRectangleRec(envItems[i].rect, envItems[i].color);
            }
        }

        for (int i = 0; i < enemiesCount; i++)
            DrawSprite(&(enemies[i].sprite), (Vector2){enemies[i].rect.x, enemies[i].rect.y});

        if (player.attackFramesCurrent > 0)
        {
            DrawTexturePro(player.sprite.texture, player.sprite.frameRec, player.rect, (Vector2){player.sprite.width / 2, player.sprite.texture.height / 2}, player.attackFramesCurrent * 10 * -1, WHITE);
        }
        else
        {
            DrawSprite(&(player.sprite), (Vector2){player.rect.x, player.rect.y});
        }

        EndMode2D();

        if (player.isAlive && activeEnemiesCount > 0)
        {
            DrawFPS(10, 10);
        }
        else if (activeEnemiesCount == 0)
        {
            winBarCurrent++;
            winBarCurrent = Clamp(winBarCurrent, 0, winBarMax);
            DrawTexture(win, 0, 0, WHITE);
            DrawRectangle(200, 270, winBarMax * 2, 60, GRAY);
            DrawRectangle(200, 270, winBarCurrent, 60, YELLOW);
            DrawText("Revenue", 230, 290, 30, BLACK);
        }
        else if (!player.isAlive)
        {
            DrawTexture(lose, 0, 0, WHITE);
        }

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow(); // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
