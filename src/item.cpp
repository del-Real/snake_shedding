#include "item.h"
#include "constants.h"

Item::Item() : pos({0, 0}), color(), bounds({0, 0, TILE_SIZE, TILE_SIZE}) {}

Item::Item(Vector2 pos, Color color, Rectangle bounds)
    : pos(pos), color(color), bounds(bounds) {}

Item::~Item() {  // Clean-up code if needed
}

void Item::Draw() {
  DrawRectangle(pos.x * TILE_SIZE, pos.y * TILE_SIZE, TILE_SIZE, TILE_SIZE,
                color);
}