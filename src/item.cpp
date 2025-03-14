#include "item.h"

Item::Item() = default;
Item::Item(const Vector2 &pos, Color color) : pos(pos), color(color) {}

void Item::Draw() const {
    DrawRectangle(static_cast<int>(pos.x * TILE_SIZE),
                  static_cast<int>(pos.y * TILE_SIZE), TILE_SIZE, TILE_SIZE,
                  color);
}
