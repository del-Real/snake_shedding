#include <stdio.h>
#include <fstream>
#include <iostream>
#include "constants.h"
#include "ekans.h"
#include "item.h"
#include "json.hpp"
#include "raylib.h"

using json = nlohmann::json;

int main(void) {
  const char title[] = "";
  bool toggleGrid = false;
  float x = 1.0f, y = 0.0f;
  int bodySize[MAX_TILES];

  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, title);

  // Load JSON file
  std::ifstream f("src/themes.json");
  json colorThemes = json::parse(f);

  // Access the "themes" array
  auto themes = colorThemes["themes"];

  // Check if there are not themes
  if (themes.empty()) {
    std::cerr << "No themes found!" << std::endl;
    return 1;
  }

  auto& firstTheme = themes[0];  // Access the first theme

  // Access the colors
  Color lightColor, regularColor, boldColor, heavyColor;

  const auto& colors = firstTheme["colors"];

  // Assign values to the color variables
  lightColor = {colors["light"]["r"], colors["light"]["g"],
                colors["light"]["b"], colors["light"]["a"]};

  regularColor = {colors["regular"]["r"], colors["regular"]["g"],
                  colors["regular"]["b"], colors["regular"]["a"]};

  boldColor = {colors["bold"]["r"], colors["bold"]["g"], colors["bold"]["b"],
               colors["bold"]["a"]};

  heavyColor = {colors["heavy"]["r"], colors["heavy"]["g"],
                colors["heavy"]["b"], colors["heavy"]["a"]};

  int score = 0;
  char windowTitle[128];  // Buffer to hold the window title

  // Random initial position
  float initItemPosX = rand() % (MAX_TILES + 1);
  float initItemPosY = rand() % (MAX_TILES + 1);

  float initEkansPosX = rand() % (MAX_TILES + 1);
  float initEkansPosY = rand() % (MAX_TILES + 1);

  Item item({initItemPosX, initItemPosY}, boldColor);
  Ekans ekans({initEkansPosX, initEkansPosY}, heavyColor, bodySize);

  float duration = 0.1f;  // Time interval for movement in seconds
  float elapsedTime = 0.0f;

  SetTargetFPS(60);
  while (!WindowShouldClose()) {
    //==========================
    // Update
    //==========================
    float dt = GetFrameTime();  // delta time

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

    // Check time for movement (grid based)
    elapsedTime += dt;

    // if (elapsedTime >= duration) {
    //   ekans.Move(x, y);
    //   elapsedTime = 0.0f;
    // }

    if (elapsedTime >= duration) {
      ekans.Move(x, y);
      elapsedTime = 0.0f;
    }

    // // Update interpolation progress
    // if (elapsedTime < duration) {
    //   float t = elapsedTime / duration;  // Normalized time (0 to 1)
    //   playerMove = Lerp(startPlayerPos, endPlayerPos, t);
    //   elapsedTime += dt;
    // } else {
    //   playerMove = endPlayerPos;
    //   timeElapsed = 0.0f;  // Reset elapsed time for the next transition
    // }

    // Check out of bounds
    if (ekans.pos.x < 0) {
      ekans.pos.x = MAX_TILES;
    }

    if (ekans.pos.y < 0) {
      ekans.pos.y = MAX_TILES;
    }

    if (ekans.pos.x > MAX_TILES) {
      ekans.pos.x = 0;
    }

    if (ekans.pos.y > MAX_TILES) {
      ekans.pos.y = 0;
    }

    // Item collision
    if (ekans.pos.x == item.pos.x && ekans.pos.y == item.pos.y) {
      item.~Item();

      score += POINTS;
    }

    // Update the window title
    snprintf(windowTitle, sizeof(windowTitle), "snake \U0001F40D Score: %d",
             score);
    SetWindowTitle(windowTitle);

    //==========================
    // Draw
    //==========================

    BeginDrawing();
    ClearBackground(lightColor);

    // Switch theme
    if (IsKeyPressed(KEY_TAB)) {
    }

    // Show/hide grid
    if (IsKeyPressed(KEY_G)) {
      toggleGrid = !toggleGrid;
    }

    if (toggleGrid) {
      for (int i = 0; i <= SCREEN_HEIGHT / TILE_SIZE; i++) {
        DrawLineV((Vector2){(float)TILE_SIZE * i, 0},
                  (Vector2){(float)TILE_SIZE * i, (float)SCREEN_HEIGHT},
                  regularColor);
      }

      for (int i = 0; i <= SCREEN_WIDTH / TILE_SIZE; i++) {
        DrawLineV((Vector2){0, (float)TILE_SIZE * i},
                  (Vector2){(float)SCREEN_WIDTH, (float)TILE_SIZE * i},
                  regularColor);
      }
    }

    item.Draw();
    ekans.Draw();

    EndDrawing();
  }

  CloseWindow();

  return 0;
}