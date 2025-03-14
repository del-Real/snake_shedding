#pragma once

#include "constants.h"
#include "raylib.h"
#include <deque>

class Ekans {
  public:
    std::deque<Vector2> body;
    Color color;

    Ekans();
    Ekans(std::deque<Vector2> body, Color color);

    void Draw() const;
    void Move(const Vector2 &dir);
    void Grow();
};
