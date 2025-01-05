#include "constants.h"
#include "raylib.h"

class Ekans {
 public:
  Vector2 pos;
  Color color;
  int bodySize[MAX_TILES];

  Ekans();
  Ekans(Vector2 pos, Color color, int bodySize[]);

  void Draw();
  void Move(float dirX, float dirY);
  void Grow();
};