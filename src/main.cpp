#include <stdio.h>
#include "constants.h"
#include "ekans.h"
#include "item.h"
#include "raylib.h"

int main(void) {
  Color lightGreen = {155, 188, 15, 255};
  Color regularGreen = {139, 172, 15, 255};
  Color boldGreen = {48, 98, 48, 255};
  Color heavyGreen = {15, 56, 15, 255};

  const char title[] = "";
  bool toggleGrid = true;
  int x = 0, y = 0;
  void CheckCollisionWithFood();

  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, title);

  int score = 0;
  char windowTitle[128];  // Buffer to hold the window title

  Item item({3, 5}, boldGreen, {3, 5});
  Ekans ekans({9, 5}, heavyGreen, {9, 5});

  float moveInterval = 0.1f;  // Time interval for movement in seconds
  float elapsedTime = 0.0f;
  float dt = 0.0f;

  SetTargetFPS(60);
  while (!WindowShouldClose()) {
    //==========================
    // Update
    //==========================
    dt = GetFrameTime();  // delta time

    // Snake movement
    if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) {
      y = -1;
      x = 0;
    }

    if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) {
      y = 1;
      x = 0;
    }
    if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) {
      x = 1;
      y = 0;
    }

    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) {
      x = -1;
      y = 0;
    }

    elapsedTime += dt;

    if (elapsedTime >= moveInterval) {
      ekans.Move(x, y);
      elapsedTime = 0.0f;
    }

    // Item collider

    // Update the window title
    snprintf(windowTitle, sizeof(windowTitle), "snake \U0001F40D Score: %d",
             score);
    SetWindowTitle(windowTitle);

    //==========================
    // Draw
    //==========================

    BeginDrawing();
    ClearBackground(lightGreen);

    if (IsKeyPressed(KEY_TAB)) {
      toggleGrid = !toggleGrid;
    }

    if (toggleGrid) {
      for (int i = 0; i <= SCREEN_HEIGHT / TILE_SIZE; i++) {
        DrawLineV((Vector2){(float)TILE_SIZE * i, 0},
                  (Vector2){(float)TILE_SIZE * i, (float)SCREEN_HEIGHT},
                  regularGreen);
      }

      for (int i = 0; i <= SCREEN_WIDTH / TILE_SIZE; i++) {
        DrawLineV((Vector2){0, (float)TILE_SIZE * i},
                  (Vector2){(float)SCREEN_WIDTH, (float)TILE_SIZE * i},
                  regularGreen);
      }
    }

    item.Draw();
    ekans.Draw();

    EndDrawing();
  }

  CloseWindow();

  return 0;
}