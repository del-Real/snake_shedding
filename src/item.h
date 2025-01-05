#include "raylib.h"

class Item {
 public:
  Vector2 pos;
  Color color;

  Item();
  Item(Vector2 pos, Color color);
  ~Item();

  void Draw();
};
