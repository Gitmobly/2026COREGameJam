#include "utils.h"
#include "graphics.h"

float ClampLo(float value, float min) { return (value < min) ? min : value; }
float ClampHi(float value, float max) { return (value > max) ? max : value; }

b32 IsOutOfBounds(Vector2 pos, float radius) {
    return (pos.x - radius <= 0)
        || (pos.y - radius <= 0)
        || (pos.x + radius >= GAME_WD)
        || (pos.y + radius >= GAME_HT);
}
