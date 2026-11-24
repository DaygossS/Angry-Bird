#include "raylib.h"

#include "resource_dir.h"
#include "WorldConvention.h"

int main()
{
    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI | FLAG_MSAA_4X_HINT);
    InitWindow(1280, 720, "SiegeLab");
    SearchAndSetResourceDir("resources");

    const cyclone::Vector3 launcher = SiegeLab::GetLauncherPosition();

    Camera3D camera = {};
    camera.position = { 0.0f, 8.0f, 18.0f };
    camera.target = { 0.0f, 1.0f, 0.0f };
    camera.up = { 0.0f, 1.0f, 0.0f };
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        BeginMode3D(camera);
        DrawPlane({ 0.0f, 0.0f, 0.0f }, { 40.0f, 20.0f }, LIGHTGRAY);
        DrawGrid(40, 1.0f);
        DrawSphere({ static_cast<float>(launcher.x), static_cast<float>(launcher.y), static_cast<float>(launcher.z) }, 0.3f, ORANGE);
        DrawCube({ 6.0f, 1.5f, 0.0f }, 0.5f, 3.0f, 0.5f, DARKBLUE);
        EndMode3D();

        DrawText("SiegeLab - setup OK (raylib + Cyclone + SiegeLabCore)", 20, 20, 20, DARKGRAY);
        DrawFPS(20, 50);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
