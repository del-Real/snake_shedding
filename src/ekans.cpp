#include "ekans.h"
#include <iostream>
#include "constants.h"
#include "item.h"

Ekans::Ekans() : pos({0, 0}), color(), bodySize{0} {}

Ekans::Ekans(Vector2 pos, Color color, int bodySize[])
    : pos(pos), color(color) {
  for (int i = 0; i < MAX_TILES; i++) {
    this->bodySize[i] = bodySize[i];
  }
}

void Ekans::Draw() {
  DrawRectangle(pos.x * TILE_SIZE, pos.y * TILE_SIZE, TILE_SIZE, TILE_SIZE,
                color);
}

void Ekans::Move(float dirX, float dirY) {
  Ekans::pos.x += dirX;
  Ekans::pos.y += dirY;
}

void Ekans::Grow() {
  DrawRectangle((pos.x + 1) * TILE_SIZE, pos.y * TILE_SIZE, TILE_SIZE,
                TILE_SIZE, color);
}