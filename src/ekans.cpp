#include "ekans.h"
#include <iostream>
#include "constants.h"
#include "item.h"

Ekans::Ekans() : pos({0, 0}), color(), bounds({0, 0, TILE_SIZE, TILE_SIZE}) {}

Ekans::Ekans(Vector2 pos, Color color, Rectangle bounds)
    : pos(pos), color(color), bounds(bounds) {}

void Ekans::Draw() {
  DrawRectangle(pos.x * TILE_SIZE, pos.y * TILE_SIZE, TILE_SIZE, TILE_SIZE,
                color);
}

void Ekans::Move(int dirX, int dirY) {
  Ekans::pos.x += dirX * MOVE_SPEED;
  Ekans::pos.y += dirY * MOVE_SPEED;

  std::cout << "Pos - x: " << pos.x << ", y: " << pos.y << std::endl;
}