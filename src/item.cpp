#include "item.h"
#include <iostream>
#include "constants.h"

Item::Item() : pos({0, 0}), color() {}

Item::Item(Vector2 pos, Color color) : pos(pos), color(color) {}

Item::~Item() {  // Clean-up code if needed
  pos.x = rand() % (MAX_TILES + 1);
  pos.y = rand() % (MAX_TILES + 1);
}

void Item::Draw() {
  DrawRectangle(pos.x * TILE_SIZE, pos.y * TILE_SIZE, TILE_SIZE, TILE_SIZE,
                color);
}