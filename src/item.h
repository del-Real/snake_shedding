#pragma once

#include "constants.h"
#include "raylib.h"

class Item {
  public:
    Vector2 pos;
    Color color;

    Item();
    Item(const Vector2 &pos, Color color);
    void Draw() const;
};
