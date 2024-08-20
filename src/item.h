#include "raylib.h"

class Item {
 public:
  Item();
  Item(Vector2 pos, Color color, Rectangle bounds);
  ~Item();

  void Draw();

 private:
  Vector2 pos;
  Color color;
  Rectangle bounds;
};
