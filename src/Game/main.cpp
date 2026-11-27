#include "raylib.h"

#include "resource_dir.h"
#include "WorldConvention.h"
#include "InputController.h"

int main()
{
    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI | FLAG_MSAA_4X_HINT);
    InitWindow(1280, 720, "SiegeLab");
    SearchAndSetResourceDir("resources");

    const cyclone::Vector3 launcher = SiegeLab::GetLauncherPosition();
    const Vector3 spherePosition = { static_cast<float>(launcher.x), static_cast<float>(launcher.y), static_cast<float>(launcher.z) };
    const float sphereRadius = 0.3f;

    Camera3D camera = {};
    camera.position = { 0.0f, 8.0f, 18.0f };
    camera.target = { 0.0f, 1.0f, 0.0f };
    camera.up = { 0.0f, 1.0f, 0.0f };
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    InputController inputController;

    while (!WindowShouldClose())
    {
        inputController.Selecting(camera, spherePosition, sphereRadius);

        BeginDrawing();
        ClearBackground(RAYWHITE);

        BeginMode3D(camera);
        DrawPlane({ 0.0f, 0.0f, 0.0f }, { 40.0f, 20.0f }, LIGHTGRAY);
        DrawGrid(40, 1.0f);
        if (inputController.IsSelected())
        {
            DrawSphere(spherePosition, sphereRadius, RED);
            DrawSphereWires(spherePosition, sphereRadius + 0.1f, 12, 12, GREEN);
        }
        else
        {
            DrawSphere(spherePosition, sphereRadius, ORANGE);
        }
     //   DrawRay(inputController.GetRay(), MAROON);
        DrawCube({ 6.0f, 1.5f, 0.0f }, 0.5f, 3.0f, 0.5f, DARKBLUE);
        EndMode3D();

        DrawText("SiegeLab - setup OK (raylib + Cyclone + SiegeLabCore)", 20, 20, 20, DARKGRAY);
        DrawText("Click on the ANGRYYY BIRD with your mouse!", 20, 80, 20, DARKGRAY);
        if (inputController.IsSelected())
        {
            DrawText("ANGRY BIRD SELECTED", (GetScreenWidth() - MeasureText("ANGRY BIRD SELECTED", 30)) / 2, 60, 30, GREEN);
        }

        DrawFPS(20, 50);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}