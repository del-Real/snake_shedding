#include "raylib.h"

class Ekans {
 public:
  Ekans();
  Ekans(Vector2 pos, Color color, Rectangle bounds);

  void Draw();
  void Move(int dirX, int dirY);
  void Eat();

 private:
  Vector2 pos;
  Color color;
  Rectangle bounds;
};